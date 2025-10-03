#pragma once

#include <string>

using namespace std;

using IRI = string;

namespace rdf {
  const IRI type = "http://www.w3.org/1999/02/22-rdf-syntax-ns#type";
} // namespace rdf

namespace rdfs {
  const IRI label = "http://www.w3.org/2000/01/rdf-schema#label";
}

namespace xsd {
  const IRI STRING = "http://www.w3.org/2001/XMLSchema#string";
  const IRI BOOLEAN = "http://www.w3.org/2001/XMLSchema#boolean";
  const IRI INTEGER = "http://www.w3.org/2001/XMLSchema#integer";
  const IRI DECIMAL = "http://www.w3.org/2001/XMLSchema#decimal";
  const IRI DOUBLE = "http://www.w3.org/2001/XMLSchema#double";
  const IRI FLOAT = "http://www.w3.org/2001/XMLSchema#float";
  const IRI DATETIME = "http://www.w3.org/2001/XMLSchema#dateTime";
  const IRI DATE = "http://www.w3.org/2001/XMLSchema#date";
  const IRI TIME = "http://www.w3.org/2001/XMLSchema#time";
} // namespace xsd

namespace cco {
  const IRI about = "https://www.commoncoreontologies.org/ont00001808";
  const IRI has_value = "https://www.commoncoreontologies.org/ont000";
  const IRI uses_reference_system = "https://www.commoncoreontologies.org/ont00001912";
  const IRI ecef = "https://www.commoncoreontologies.org/ont00001648";
  const IRI lat = "https://www.commoncoreontologies.org/ont00001766";
  const IRI lon = "https://www.commoncoreontologies.org/ont00001764";
  const IRI height = "https://www.commoncoreontologies.org/ont000";
} // namespace cco

namespace imh {
  const IRI image = "http://ontology.mil/foundry/IMH_0000078";
  const IRI tai_timestamp = "http://ontology.mil/foundry/IMH_0001416";
  const IRI ephemeris_series = "http://ontology.mil/foundry/IMH_0001646";
  const IRI ecef_coordinate = "http://ontology.mil/foundry/IMH_0002084";
  const IRI has_x = "http://ontology.mil/foundry/IMH_0001430";
  const IRI has_y = "http://ontology.mil/foundry/IMH_0001474";
  const IRI has_z = "http://ontology.mil/foundry/IMH_0001447";
  const IRI has_unit = "http://ontology.mil/foundry/IMH_0001159";
  const IRI wgs84_degrees = "http://ontology.mil/foundry/IMH_0001081";
  const IRI wgs84_radians = "http://ontology.mil/foundry/IMH_0001400";
  const IRI raster_bounds_correspondence_group = "http://ontology.mil/foundry/IMH_0001634";
  const IRI ccs_coordinate = "http://ontology.mil/foundry/IMH_0001644";
  const IRI column_number = "http://ontology.mil/foundry/IMH_0001630";
  const IRI row_number = "http://ontology.mil/foundry/IMH_0001626";
  const IRI correspondence_target = "http://ontology.mil/foundry/IMH_0001667";
  const IRI correspondence_source = "http://ontology.mil/foundry/IMH_0001642";
  const IRI at = "http://ontology.mil/foundry/IMH_0001161";
} // namespace imh

namespace qunit {
  const IRI meter = "http://qudt.org/vocab/unit/M";
}

namespace geosparql {
  const IRI geometry = "http://www.opengis.net/ont/geosparql#geometry";
  const IRI asWKT = "http://www.opengis.net/ont/geosparql#asWKT";
} // namespace geosparql