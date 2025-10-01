#pragma once
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

class CSVReader {
public:
  // Load and parse the entire CSV file
  explicit CSVReader(const string &filename);

  // Getters
  vector<string> get_row(size_t row_index) const;
  string get_cell(size_t row_index, const string &header) const;
  vector<string> get_column(const string &header) const;
  size_t get_row_count() const;    // number of data rows (excluding header)
  size_t get_column_count() const; // number of columns
  bool has_header(const string &header) const;

private:
  vector<string> m_headers;
  unordered_map<string, size_t> m_headerIndex;
  vector<vector<string>> m_data; // all rows of CSV
};
