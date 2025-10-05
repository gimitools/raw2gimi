#include "rdf_converter.h"
#include "model/rdf_literal.h"
#include "model/resource.h"
#include <sstream>

using namespace gimi;
using namespace ido;

// Constructor
RDFConverter::RDFConverter() {}

// API
void RDFConverter::export_to_file(const string &filename) {
  m_redland.export_to_file(filename);
}

void RDFConverter::add_label(const IRI &subject, const string &label) {
  add_triple(subject, rdfs::label, RDFLiteral(label));
}

void RDFConverter::add_label(const Resource &resource) {
  add_label(resource.get_iri(), resource.get_label());
}

void RDFConverter::add_image(const IRI &image_iri) {
  cout << "Adding image IRI: " << image_iri << endl;
  add_triple(image_iri, rdf::type, imh::image);
}

IRI RDFConverter::add_timestamp(uint64_t tai_time) {
  IRI timestamp_iri = Resource::generate_iri();
  add_triple(timestamp_iri, rdf::type, imh::tai_timestamp);
  add_triple(timestamp_iri, cco::has_value, RDFLiteral(tai_time));
  add_label(timestamp_iri, "TAI Timestamp");
  // TODO: add human readable date as label
  return timestamp_iri;
}

void RDFConverter::add_coordinate(const Coordinate &coordinate) {
  IRI cord = coordinate.get_iri();
  RDFLiteral lat(coordinate.get_latitude());
  RDFLiteral lon(coordinate.get_longitude());

  add_triple(cord, rdf::type, imh::wgs84_degrees);
  add_triple(cord, cco::lat, lat);
  add_triple(cord, cco::lon, lon);
  add_label(cord, "Ground Coordinate");
}

void RDFConverter::add_coordinates(const BoundingBox &bbox) {
  add_coordinate(bbox.get_top_left());
  add_coordinate(bbox.get_top_right());
  add_coordinate(bbox.get_bottom_left());
  add_coordinate(bbox.get_bottom_right());
}

void RDFConverter::add_points(const CornerPoints &corners) {
  add_point(corners.top_left);
  add_point(corners.top_right);
  add_point(corners.bottom_left);
  add_point(corners.bottom_right);
}

void RDFConverter::add_point(const Point &point_object) {
  IRI point = point_object.get_iri();
  RDFLiteral x(point_object.get_x());
  RDFLiteral y(point_object.get_y());

  add_triple(point, rdf::type, imh::ccs_coordinate);
  add_triple(point, imh::row_number, y);
  add_triple(point, imh::column_number, x);
  add_label(point, "Image Coordinate");
}

void RDFConverter::add_correspondence(const Correspondence &correspondence) {
  IRI correspondence_iri = correspondence.get_iri();
  IRI ground_cord = correspondence.coordinate.get_iri();
  IRI image_cord = correspondence.point.get_iri();

  // Correspondence
  add_triple(correspondence_iri, rdf::type, imh::raster_bounds_correspondence_group);
  add_triple(correspondence_iri, imh::correspondence_source, image_cord);
  add_triple(correspondence_iri, imh::correspondence_target, ground_cord);
  add_label(correspondence_iri, "Correspondence");
}

void RDFConverter::add_correspondences(const vector<Correspondence> &correspondences) {
  for (const auto &correspondence : correspondences) {
    add_correspondence(correspondence);
  }
}

void RDFConverter::add_correspondence_group(IRI content_id,
                                            const CorrespondenceGroup &group) {
  IRI group_i = group.iri();
  vector<Coordinate> coordinates = group.get_coordinates();
  string wkt_polygon_string = make_wkt_crs84_polygon(coordinates);
  RDFLiteral wkt_polygon(wkt_polygon_string, geosparql::wktLiteral);

  for (const auto &correspondence : group.correspondences) {
    add_triple(group_i, imh::has_correspondence, correspondence.iri());
  }

  add_triple(group_i, rdf::type, imh::raster_bounds_correspondence_group);
  add_triple(group_i, cco::about, content_id);

  add_triple(group_i, rdf::type, geosparql::geometry);
  add_triple(group_i, geosparql::asWKT, wkt_polygon);

  // TODO: link corerespondences so they can be queried in the correct order.
}

// Generators

vector<Correspondence> RDFConverter::create_correspondences(const CornerPoints &image_corners,
                                                            const BoundingBox &ground_bbox) {
  vector<Correspondence> correspondences;

  Correspondence top_left(ground_bbox.get_top_left(), image_corners.top_left);
  Correspondence top_right(ground_bbox.get_top_right(), image_corners.top_right);
  Correspondence bottom_left(ground_bbox.get_bottom_left(), image_corners.bottom_left);
  Correspondence bottom_right(ground_bbox.get_bottom_right(), image_corners.bottom_right);

  correspondences.push_back(top_left);
  correspondences.push_back(top_right);
  correspondences.push_back(bottom_left);
  correspondences.push_back(bottom_right);

  return correspondences;
}

IRI RDFConverter::generate_correspondence_group(IRI content_id,
                                                vector<IRI> correspondences,
                                                vector<Coordinate> ground_coordinates,
                                                IRI timestamp) {
  IRI correspondence_group = Resource::generate_iri();
  string wkt_polygon = make_wkt_crs84_polygon(ground_coordinates);
  RDFLiteral wkt_literal(wkt_polygon, geosparql::wktLiteral);

  add_triple(correspondence_group, rdf::type, imh::raster_bounds_correspondence_group);
  add_triple(correspondence_group, imh::at, timestamp);
  add_triple(correspondence_group, cco::about, content_id);
  add_label(correspondence_group, "Correspondence Group");

  for (const auto &correspondence : correspondences) {
    add_triple(correspondence_group, imh::has_correspondence, correspondence);
  }

  // For GeoSPARQL:
  add_triple(correspondence_group, rdf::type, geosparql::geometry);
  add_triple(correspondence_group, geosparql::asWKT, wkt_literal);

  return correspondence_group;
}

string RDFConverter::make_wkt_crs84_polygon(const vector<Coordinate> &coords) {
  if (coords.size() < 3) {
    throw std::invalid_argument("WKT polygon requires at least 3 coordinates");
  }

  const Coordinate &first = coords.front();
  const Coordinate &last = coords.back();
  const bool is_closed = (first == last);

  std::string wkt;
  wkt.reserve(32 * coords.size()); // rough preallocate
  wkt += "POLYGON((";

  auto append_pair = [&](const Coordinate &c, bool need_comma) {
    wkt += std::to_string(c.get_longitude());
    wkt += ' ';
    wkt += std::to_string(c.get_latitude());
    if (need_comma)
      wkt += ',';
  };

  const std::size_t n = coords.size();
  for (std::size_t i = 0; i < n; ++i) {
    append_pair(coords[i], true);
  }

  if (!is_closed) {
    append_pair(first, false);
  } else {
    // we already added commas after each of the n items; remove the extra comma
    if (!wkt.empty() && wkt.back() == ',')
      wkt.pop_back();
  }

  wkt += "))";
  return wkt;
}

// Getters
Coordinate RDFConverter::get_ground_coordinate(const IRI &correspondence) {
  return Coordinate(0, 0); // TODO
}

// Protected
void RDFConverter::add_triple(const IRI &subject, const IRI &predicate, const IRI &object) {
  m_redland.add_triple(subject, predicate, object);
}

void RDFConverter::add_triple(const IRI &subject, const IRI &predicate, const RDFLiteral &object) {
  m_redland.add_triple(subject, predicate, object);
}
