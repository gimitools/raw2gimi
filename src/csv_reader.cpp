#include "csv_reader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

CSVReader::CSVReader(const string &filename) {
  ifstream file(filename);
  if (!file.is_open()) {
    throw runtime_error("Could not open file: " + filename);
  }

  string line;

  // ---- Parse header row ----
  if (!getline(file, line)) {
    throw runtime_error("Empty CSV file: " + filename);
  }

  stringstream header_stream(line);
  string cell;
  size_t idx = 0;
  while (getline(header_stream, cell, ',')) {
    m_headers.push_back(cell);
    m_headerIndex[cell] = idx++;
  }

  // ---- Parse data rows ----
  while (getline(file, line)) {
    stringstream row_stream(line);
    vector<string> row;
    while (getline(row_stream, cell, ',')) {
      row.push_back(cell);
    }
    m_data.push_back(row);
  }
}

vector<string> CSVReader::get_row(size_t row_index) const {
  if (row_index >= m_data.size()) {
    throw out_of_range("Row index out of range");
  }
  return m_data[row_index];
}

string CSVReader::get_cell(size_t row_index, const string &header) const {
  auto it = m_headerIndex.find(header);
  if (it == m_headerIndex.end()) {
    throw invalid_argument("Header not found: " + header);
  }
  size_t col = it->second;
  if (row_index >= m_data.size() || col >= m_data[row_index].size()) {
    throw out_of_range("Cell out of range");
  }
  return m_data[row_index][col];
}

vector<string> CSVReader::get_column(const string &header) const {
  auto it = m_headerIndex.find(header);
  if (it == m_headerIndex.end()) {
    throw invalid_argument("Header not found: " + header);
  }
  size_t col = it->second;

  vector<string> column;
  for (const auto &row : m_data) {
    if (col < row.size()) {
      column.push_back(row[col]);
    } else {
      column.push_back(""); // missing cell
    }
  }
  return column;
}

size_t CSVReader::get_row_count() const {
  return m_data.size();
}

size_t CSVReader::get_column_count() const {
  return m_headers.size();
}

bool CSVReader::has_header(const string &header) const {
  return m_headerIndex.count(header) > 0;
}
