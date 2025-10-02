#pragma once

#include "model/iri.h"
#include "model/rdf_literal.h"
#include <librdf.h>

using namespace std;

namespace gimi {

  class RedlandWrapper {

  public: // Constructors
    RedlandWrapper();
    ~RedlandWrapper();

  public: // API
    void add_triple(const IRI &subject, const IRI &predicate, const IRI &object);
    void add_triple(const IRI &subject, const IRI &predicate, const RDFLiteral &object);
    void export_to_file(const string &filename);
    static void debug();

  private: // Helpers
    void register_namespaces(librdf_serializer *);
    void register_namespace(librdf_serializer *, string prefix, string ns);

  private:
    librdf_world *m_world;
    librdf_storage *m_storage;
    librdf_model *m_model; // graph
  };
} // namespace gimi