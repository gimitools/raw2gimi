#include "redland_wrapper.h"
#include "error_handler.h"
#include <librdf.h>

using namespace gimi;

// Constructors

RedlandWrapper::RedlandWrapper() {
  m_world = librdf_new_world();
  if (!m_world) {
    throw_error("Failed to create new RDF world");
  }

  m_storage = librdf_new_storage(m_world, "memory", NULL, NULL);
  m_model = librdf_new_model(m_world, m_storage, NULL);
}

RedlandWrapper::~RedlandWrapper() {
  if (m_world) {
    librdf_free_world(m_world);
    m_world = nullptr;
  }
}

// API

void RedlandWrapper::add_triple(const IRI &subject, const IRI &predicate, const IRI &object) {
  // Create Nodes
  librdf_node *s = librdf_new_node_from_uri_string(m_world, (const unsigned char *)subject.c_str());
  librdf_node *p = librdf_new_node_from_uri_string(m_world, (const unsigned char *)predicate.c_str());
  librdf_node *o = librdf_new_node_from_uri_string(m_world, (const unsigned char *)object.c_str());

  // Create Triple
  librdf_statement *triple = librdf_new_statement_from_nodes(m_world, s, p, o);

  // Add Triple to Graph
  librdf_model_add_statement(m_model, triple);

  // Cleanup
  librdf_free_statement(triple);
}

void RedlandWrapper::add_triple(const IRI &subject, const IRI &predicate, const RDFLiteral &object) {
  librdf_node *s = librdf_new_node_from_uri_string(m_world, (const unsigned char *)subject.c_str());
  librdf_node *p = librdf_new_node_from_uri_string(m_world, (const unsigned char *)predicate.c_str());
  string value = object.get_value();
  IRI datatype = object.get_datatype();
  librdf_uri *datatype_uri = librdf_new_uri(m_world, (const unsigned char *)datatype.c_str());

  librdf_node *o = librdf_new_node_from_typed_literal(m_world, (const unsigned char *)value.c_str(), NULL,
                                                      datatype_uri);

  librdf_statement *triple = librdf_new_statement_from_nodes(m_world, s, p, o);

  librdf_model_add_statement(m_model, triple);

  librdf_free_statement(triple);
}

void RedlandWrapper::export_to_file(const string &filename) {
  librdf_serializer *serializer = librdf_new_serializer(m_world, "turtle", NULL, NULL);

  register_namespaces(serializer);

  librdf_serializer_serialize_model_to_file(serializer, filename.c_str(), NULL, m_model);
  librdf_free_serializer(serializer);
}

// Helpers

void RedlandWrapper::register_namespaces(librdf_serializer *serializer) {
  register_namespace(serializer, "cco", "https://www.commoncoreontologies.org/");
  register_namespace(serializer, "xsd", "http://www.w3.org/2001/XMLSchema#");
  // register_namespace(serializer, "imh", "http://ontology.mil/foundry/");
  // register_namespace(serializer, "imh", "http://ontology.mil/foundry/IMH_");
  register_namespace(serializer, "imh", "http://ontology.mil/foundry/IMH");
  register_namespace(serializer, "obi", "http://purl.obolibrary.org/obo/");
  register_namespace(serializer, "qunit", "http://qudt.org/vocab/unit/");
}

void RedlandWrapper::register_namespace(librdf_serializer *serializer, string prefix, string ns) {
  librdf_uri *ns_uri = librdf_new_uri(m_world, (const unsigned char *)ns.c_str());
  librdf_serializer_set_namespace(serializer, ns_uri, prefix.c_str());
  librdf_free_uri(ns_uri);
}

void RedlandWrapper::debug() {
  librdf_world *world = librdf_new_world();
  librdf_world_open(world);

  // Create a new storage and model
  librdf_storage *storage = librdf_new_storage(world, "memory", NULL, NULL);
  librdf_model *model = librdf_new_model(world, storage, NULL);

  // Define some URIs
  librdf_uri *subject_uri = librdf_new_uri(world, (const unsigned char *)"http://example.org/subject");
  librdf_uri *predicate_uri = librdf_new_uri(world, (const unsigned char *)"http://example.org/predicate");
  librdf_node *object_node = librdf_new_node_from_literal(world, (const unsigned char *)"Hello world", NULL, 0);

  // Create the statement and add it to the model
  librdf_statement *statement = librdf_new_statement_from_nodes(world,
                                                                librdf_new_node_from_uri(world, subject_uri),
                                                                librdf_new_node_from_uri(world, predicate_uri),
                                                                object_node);
  librdf_model_add_statement(model, statement);

  // Serialize the model to a Turtle file
  librdf_serializer *serializer = librdf_new_serializer(world, "turtle", NULL, NULL);
  FILE *outfile = fopen("output.ttl", "w");
  if (!outfile) {
    throw_error("Error opening file for writing");
  }

  librdf_uri *base_uri = librdf_new_uri(world, (const unsigned char *)"http://example.org/");
  librdf_serializer_serialize_model_to_file_handle(serializer, outfile, base_uri, model);
  fclose(outfile);

  // Clean up
  librdf_free_serializer(serializer);
  librdf_free_statement(statement);
  librdf_free_uri(subject_uri);
  librdf_free_uri(predicate_uri);
  librdf_free_uri(base_uri);
  librdf_free_model(model);
  librdf_free_storage(storage);
  librdf_free_world(world);
}