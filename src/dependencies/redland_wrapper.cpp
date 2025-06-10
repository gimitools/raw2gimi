#include "redland_wrapper.h"
#include "error_handler.h"
#include <librdf.h>

using namespace gimi;

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