#pragma once

#include <string>

using namespace std;

using IRI = string;

namespace rdf {
  const IRI type = "http://www.w3.org/1999/02/22-rdf-syntax-ns#type";
} // namespace rdf

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
} // namespace imh

namespace qunit {
  const IRI meter = "http://qudt.org/vocab/unit/M";
}