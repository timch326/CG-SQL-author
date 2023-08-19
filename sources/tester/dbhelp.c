/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "dbhelp.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-warning-option"
#pragma clang diagnostic ignored "-Wbitwise-op-parentheses"
#pragma clang diagnostic ignored "-Wshift-op-parentheses"
#pragma clang diagnostic ignored "-Wlogical-not-parentheses"
#pragma clang diagnostic ignored "-Wlogical-op-parentheses"
#pragma clang diagnostic ignored "-Wliteral-conversion"
#pragma clang diagnostic ignored "-Wunused-but-set-variable"
#pragma clang diagnostic ignored "-Wunused-function"
extern cql_object_ref _Nonnull cql_partition_create(void);
extern cql_bool cql_partition_cursor(cql_object_ref _Nonnull p, cql_dynamic_cursor *_Nonnull key, cql_dynamic_cursor *_Nonnull value);
extern cql_object_ref _Nonnull cql_extract_partition(cql_object_ref _Nonnull p, cql_dynamic_cursor *_Nonnull key);
extern cql_object_ref _Nonnull cql_string_dictionary_create(void);
extern cql_bool cql_string_dictionary_add(cql_object_ref _Nonnull dict, cql_string_ref _Nonnull key, cql_string_ref _Nonnull value);
extern cql_string_ref _Nullable cql_string_dictionary_find(cql_object_ref _Nonnull dict, cql_string_ref _Nullable key);
extern cql_string_ref _Nonnull cql_cursor_format(cql_dynamic_cursor *_Nonnull C);
extern CQL_WARN_UNUSED cql_code dbhelp_source(sqlite3 *_Nonnull _db_, sqlite3_stmt *_Nullable *_Nonnull _result_stmt);


//
// This file is auto-generated by dbhelp.sql, it is checked in just
// in case CQL is broken by a change.  The Last Known Good Verifier
// can be used to verify the tests pass again, or report failures
// while things are still otherwise broken.  Rebuild with 'regen.sh'
//


// Generated from dbhelp.sql:40

/*
CREATE PROC dbhelp_setup ()
BEGIN
  CREATE TABLE test_output(
    line INTEGER NOT NULL,
    data TEXT NOT NULL
  );
  CREATE INDEX __idx__test_lines ON test_output (line);
  CREATE TABLE source_input(
    line INTEGER NOT NULL,
    data TEXT NOT NULL
  );
  CREATE INDEX __idx__source_lines ON source_input (line);
END;
*/

#define _PROC_ "dbhelp_setup"
CQL_WARN_UNUSED cql_code dbhelp_setup(sqlite3 *_Nonnull _db_) {
  cql_code _rc_ = SQLITE_OK;
  cql_error_prepare();

  _rc_ = cql_exec(_db_,
    "CREATE TABLE test_output( "
      "line INTEGER NOT NULL, "
      "data TEXT NOT NULL "
    ")");
  if (_rc_ != SQLITE_OK) { cql_error_trace(); goto cql_cleanup; }
  _rc_ = cql_exec(_db_,
    "CREATE INDEX __idx__test_lines ON test_output (line)");
  if (_rc_ != SQLITE_OK) { cql_error_trace(); goto cql_cleanup; }
  _rc_ = cql_exec(_db_,
    "CREATE TABLE source_input( "
      "line INTEGER NOT NULL, "
      "data TEXT NOT NULL "
    ")");
  if (_rc_ != SQLITE_OK) { cql_error_trace(); goto cql_cleanup; }
  _rc_ = cql_exec(_db_,
    "CREATE INDEX __idx__source_lines ON source_input (line)");
  if (_rc_ != SQLITE_OK) { cql_error_trace(); goto cql_cleanup; }
  _rc_ = SQLITE_OK;

cql_cleanup:
  cql_error_report();
  return _rc_;
}
#undef _PROC_

// Generated from dbhelp.sql:50

/*
CREATE PROC dbhelp_prev_line (line_ INTEGER NOT NULL, OUT prev INTEGER NOT NULL)
BEGIN
  BEGIN TRY
    SET prev := ( SELECT ifnull(max(line), -1)
      FROM test_output
      WHERE line < line_ );
  END TRY;
  BEGIN CATCH
    SET prev := 0;
  END CATCH;
END;
*/

#define _PROC_ "dbhelp_prev_line"
CQL_WARN_UNUSED cql_code dbhelp_prev_line(sqlite3 *_Nonnull _db_, cql_int32 line_, cql_int32 *_Nonnull prev) {
  cql_contract_argument_notnull((void *)prev, 2);

  cql_code _rc_ = SQLITE_OK;
  cql_error_prepare();
  sqlite3_stmt *_temp_stmt = NULL;

  *prev = 0; // set out arg to non-garbage
  // try
  {
    _rc_ = cql_prepare(_db_, &_temp_stmt,
      "SELECT ifnull(max(line), -1) "
        "FROM test_output "
        "WHERE line < ?");
    cql_multibind(&_rc_, _db_, &_temp_stmt, 1,
                  CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_INT32, line_);
    if (_rc_ != SQLITE_OK) { cql_error_trace(); goto catch_start_1; }
    _rc_ = sqlite3_step(_temp_stmt);
    if (_rc_ != SQLITE_ROW) { cql_error_trace(); goto catch_start_1; }
      *prev = sqlite3_column_int(_temp_stmt, 0);
    cql_finalize_stmt(&_temp_stmt);
    goto catch_end_1;
  }
  catch_start_1: {
    *prev = 0;
  }
  catch_end_1:;
  _rc_ = SQLITE_OK;

  cql_error_report();
  cql_finalize_stmt(&_temp_stmt);
  return _rc_;
}
#undef _PROC_

// Generated from dbhelp.sql:57

/*
CREATE PROC dbhelp_add (line INTEGER NOT NULL, data TEXT NOT NULL)
BEGIN
  INSERT INTO test_output(line, data) VALUES(line, data);
END;
*/

#define _PROC_ "dbhelp_add"
CQL_WARN_UNUSED cql_code dbhelp_add(sqlite3 *_Nonnull _db_, cql_int32 line, cql_string_ref _Nonnull data) {
  cql_contract_argument_notnull((void *)data, 2);

  cql_code _rc_ = SQLITE_OK;
  cql_error_prepare();
  sqlite3_stmt *_temp_stmt = NULL;

  _rc_ = cql_prepare(_db_, &_temp_stmt,
    "INSERT INTO test_output(line, data) VALUES(?, ?)");
  cql_multibind(&_rc_, _db_, &_temp_stmt, 2,
                CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_INT32, line,
                CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_STRING, data);
  if (_rc_ != SQLITE_OK) { cql_error_trace(); goto cql_cleanup; }
  _rc_ = sqlite3_step(_temp_stmt);
  if (_rc_ != SQLITE_DONE) { cql_error_trace(); goto cql_cleanup; }
  cql_finalize_stmt(&_temp_stmt);
  _rc_ = SQLITE_OK;

cql_cleanup:
  cql_error_report();
  cql_finalize_stmt(&_temp_stmt);
  return _rc_;
}
#undef _PROC_

// Generated from dbhelp.sql:64

/*
CREATE PROC dbhelp_add_source (line INTEGER NOT NULL, data TEXT NOT NULL)
BEGIN
  INSERT INTO source_input(line, data) VALUES(line, data);
END;
*/

#define _PROC_ "dbhelp_add_source"
CQL_WARN_UNUSED cql_code dbhelp_add_source(sqlite3 *_Nonnull _db_, cql_int32 line, cql_string_ref _Nonnull data) {
  cql_contract_argument_notnull((void *)data, 2);

  cql_code _rc_ = SQLITE_OK;
  cql_error_prepare();
  sqlite3_stmt *_temp_stmt = NULL;

  _rc_ = cql_prepare(_db_, &_temp_stmt,
    "INSERT INTO source_input(line, data) VALUES(?, ?)");
  cql_multibind(&_rc_, _db_, &_temp_stmt, 2,
                CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_INT32, line,
                CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_STRING, data);
  if (_rc_ != SQLITE_OK) { cql_error_trace(); goto cql_cleanup; }
  _rc_ = sqlite3_step(_temp_stmt);
  if (_rc_ != SQLITE_DONE) { cql_error_trace(); goto cql_cleanup; }
  cql_finalize_stmt(&_temp_stmt);
  _rc_ = SQLITE_OK;

cql_cleanup:
  cql_error_report();
  cql_finalize_stmt(&_temp_stmt);
  return _rc_;
}
#undef _PROC_

// Generated from dbhelp.sql:74

/*
CREATE PROC dbhelp_dump_output (line_ INTEGER NOT NULL)
BEGIN
  DECLARE C CURSOR FOR SELECT *
    FROM test_output
    WHERE line = line_;
  LOOP FETCH C
  BEGIN
    CALL printf('%s', C.data);
  END;
END;
*/

#define _PROC_ "dbhelp_dump_output"

typedef struct dbhelp_dump_output_C_row {
  cql_bool _has_row_;
  cql_uint16 _refs_count_;
  cql_uint16 _refs_offset_;
  cql_int32 line;
  cql_string_ref _Nonnull data;
} dbhelp_dump_output_C_row;

#define dbhelp_dump_output_C_refs_offset cql_offsetof(dbhelp_dump_output_C_row, data) // count = 1
CQL_WARN_UNUSED cql_code dbhelp_dump_output(sqlite3 *_Nonnull _db_, cql_int32 line_) {
  cql_code _rc_ = SQLITE_OK;
  cql_error_prepare();
  sqlite3_stmt *C_stmt = NULL;
  dbhelp_dump_output_C_row C = { ._refs_count_ = 1, ._refs_offset_ = dbhelp_dump_output_C_refs_offset };

  _rc_ = cql_prepare(_db_, &C_stmt,
    "SELECT line, data "
      "FROM test_output "
      "WHERE line = ?");
  cql_multibind(&_rc_, _db_, &C_stmt, 1,
                CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_INT32, line_);
  if (_rc_ != SQLITE_OK) { cql_error_trace(); goto cql_cleanup; }
  for (;;) {
    _rc_ = sqlite3_step(C_stmt);
    C._has_row_ = _rc_ == SQLITE_ROW;
    cql_multifetch(_rc_, C_stmt, 2,
                   CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_INT32, &C.line,
                   CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_STRING, &C.data);
    if (_rc_ != SQLITE_ROW && _rc_ != SQLITE_DONE) { cql_error_trace(); goto cql_cleanup; }
    if (!C._has_row_) break;
    cql_alloc_cstr(_cstr_1, C.data);
    printf("%s", _cstr_1);
    cql_free_cstr(_cstr_1, C.data);
  }
  _rc_ = SQLITE_OK;

cql_cleanup:
  cql_error_report();
  cql_finalize_stmt(&C_stmt);
  cql_teardown_row(C);
  return _rc_;
}
#undef _PROC_

// Generated from dbhelp.sql:93

/*
CREATE PROC dbhelp_find (line_ INTEGER NOT NULL, pattern TEXT NOT NULL, OUT search_line INTEGER NOT NULL, OUT found INTEGER NOT NULL)
BEGIN
  SET search_line := ( SELECT line
    FROM test_output
    WHERE line >= line_
  LIMIT 1 );
  SET found := ( SELECT count(*)
    FROM test_output
    WHERE line = search_line AND data LIKE pattern );
END;
*/

#define _PROC_ "dbhelp_find"
CQL_WARN_UNUSED cql_code dbhelp_find(sqlite3 *_Nonnull _db_, cql_int32 line_, cql_string_ref _Nonnull pattern, cql_int32 *_Nonnull search_line, cql_int32 *_Nonnull found) {
  cql_contract_argument_notnull((void *)pattern, 2);
  cql_contract_argument_notnull((void *)search_line, 3);
  cql_contract_argument_notnull((void *)found, 4);

  cql_code _rc_ = SQLITE_OK;
  cql_error_prepare();
  sqlite3_stmt *_temp_stmt = NULL;

  *search_line = 0; // set out arg to non-garbage
  *found = 0; // set out arg to non-garbage
  _rc_ = cql_prepare(_db_, &_temp_stmt,
    "SELECT line "
      "FROM test_output "
      "WHERE line >= ? "
    "LIMIT 1");
  cql_multibind(&_rc_, _db_, &_temp_stmt, 1,
                CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_INT32, line_);
  if (_rc_ != SQLITE_OK) { cql_error_trace(); goto cql_cleanup; }
  _rc_ = sqlite3_step(_temp_stmt);
  if (_rc_ != SQLITE_ROW) { cql_error_trace(); goto cql_cleanup; }
    *search_line = sqlite3_column_int(_temp_stmt, 0);
  cql_finalize_stmt(&_temp_stmt);
  _rc_ = cql_prepare(_db_, &_temp_stmt,
    "SELECT count(*) "
      "FROM test_output "
      "WHERE line = ? AND data LIKE ?");
  cql_multibind(&_rc_, _db_, &_temp_stmt, 2,
                CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_INT32, *search_line,
                CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_STRING, pattern);
  if (_rc_ != SQLITE_OK) { cql_error_trace(); goto cql_cleanup; }
  _rc_ = sqlite3_step(_temp_stmt);
  if (_rc_ != SQLITE_ROW) { cql_error_trace(); goto cql_cleanup; }
    *found = sqlite3_column_int(_temp_stmt, 0);
  cql_finalize_stmt(&_temp_stmt);
  _rc_ = SQLITE_OK;

cql_cleanup:
  cql_error_report();
  cql_finalize_stmt(&_temp_stmt);
  return _rc_;
}
#undef _PROC_

// Generated from dbhelp.sql:103

/*
CREATE PROC dbhelp_dump_source (line1 INTEGER NOT NULL, line2 INTEGER NOT NULL)
BEGIN
  DECLARE C CURSOR FOR SELECT *
    FROM source_input
    WHERE line > line1 AND line <= line2;
  LOOP FETCH C
  BEGIN
    CALL printf('%s', C.data);
  END;
END;
*/

#define _PROC_ "dbhelp_dump_source"

typedef struct dbhelp_dump_source_C_row {
  cql_bool _has_row_;
  cql_uint16 _refs_count_;
  cql_uint16 _refs_offset_;
  cql_int32 line;
  cql_string_ref _Nonnull data;
} dbhelp_dump_source_C_row;

#define dbhelp_dump_source_C_refs_offset cql_offsetof(dbhelp_dump_source_C_row, data) // count = 1
CQL_WARN_UNUSED cql_code dbhelp_dump_source(sqlite3 *_Nonnull _db_, cql_int32 line1, cql_int32 line2) {
  cql_code _rc_ = SQLITE_OK;
  cql_error_prepare();
  sqlite3_stmt *C_stmt = NULL;
  dbhelp_dump_source_C_row C = { ._refs_count_ = 1, ._refs_offset_ = dbhelp_dump_source_C_refs_offset };

  _rc_ = cql_prepare(_db_, &C_stmt,
    "SELECT line, data "
      "FROM source_input "
      "WHERE line > ? AND line <= ?");
  cql_multibind(&_rc_, _db_, &C_stmt, 2,
                CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_INT32, line1,
                CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_INT32, line2);
  if (_rc_ != SQLITE_OK) { cql_error_trace(); goto cql_cleanup; }
  for (;;) {
    _rc_ = sqlite3_step(C_stmt);
    C._has_row_ = _rc_ == SQLITE_ROW;
    cql_multifetch(_rc_, C_stmt, 2,
                   CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_INT32, &C.line,
                   CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_STRING, &C.data);
    if (_rc_ != SQLITE_ROW && _rc_ != SQLITE_DONE) { cql_error_trace(); goto cql_cleanup; }
    if (!C._has_row_) break;
    cql_alloc_cstr(_cstr_2, C.data);
    printf("%s", _cstr_2);
    cql_free_cstr(_cstr_2, C.data);
  }
  _rc_ = SQLITE_OK;

cql_cleanup:
  cql_error_report();
  cql_finalize_stmt(&C_stmt);
  cql_teardown_row(C);
  return _rc_;
}
#undef _PROC_

// Generated from dbhelp.sql:109

/*
CREATE PROC dbhelp_source ()
BEGIN
  SELECT *
    FROM source_input;
END;
*/

#define _PROC_ "dbhelp_source"
static int32_t dbhelp_source_perf_index;

cql_string_proc_name(dbhelp_source_stored_procedure_name, "dbhelp_source");

typedef struct dbhelp_source_row {
  cql_int32 line;
  cql_string_ref _Nonnull data;
} dbhelp_source_row;

cql_int32 dbhelp_source_get_line(dbhelp_source_result_set_ref _Nonnull result_set, cql_int32 row) {
  dbhelp_source_row *data = (dbhelp_source_row *)cql_result_set_get_data((cql_result_set_ref)result_set);
  return data[row].line;
}

cql_string_ref _Nonnull dbhelp_source_get_data(dbhelp_source_result_set_ref _Nonnull result_set, cql_int32 row) {
  dbhelp_source_row *data = (dbhelp_source_row *)cql_result_set_get_data((cql_result_set_ref)result_set);
  return data[row].data;
}

uint8_t dbhelp_source_data_types[dbhelp_source_data_types_count] = {
  CQL_DATA_TYPE_INT32 | CQL_DATA_TYPE_NOT_NULL, // line
  CQL_DATA_TYPE_STRING | CQL_DATA_TYPE_NOT_NULL, // data
};

#define dbhelp_source_refs_offset cql_offsetof(dbhelp_source_row, data) // count = 1

static cql_uint16 dbhelp_source_col_offsets[] = { 2,
  cql_offsetof(dbhelp_source_row, line),
  cql_offsetof(dbhelp_source_row, data)
};

cql_int32 dbhelp_source_result_count(dbhelp_source_result_set_ref _Nonnull result_set) {
  return cql_result_set_get_count((cql_result_set_ref)result_set);
}

CQL_WARN_UNUSED cql_code dbhelp_source_fetch_results(sqlite3 *_Nonnull _db_, dbhelp_source_result_set_ref _Nullable *_Nonnull result_set) {
  sqlite3_stmt *stmt = NULL;
  cql_profile_start(CRC_dbhelp_source, &dbhelp_source_perf_index);
  cql_code rc = dbhelp_source(_db_, &stmt);
  cql_fetch_info info = {
    .rc = rc,
    .db = _db_,
    .stmt = stmt,
    .data_types = dbhelp_source_data_types,
    .col_offsets = dbhelp_source_col_offsets,
    .refs_count = 1,
    .refs_offset = dbhelp_source_refs_offset,
    .encode_context_index = -1,
    .rowsize = sizeof(dbhelp_source_row),
    .crc = CRC_dbhelp_source,
    .perf_index = &dbhelp_source_perf_index,
  };
  return cql_fetch_all_results(&info, (cql_result_set_ref *)result_set);
}

CQL_WARN_UNUSED cql_code dbhelp_source(sqlite3 *_Nonnull _db_, sqlite3_stmt *_Nullable *_Nonnull _result_stmt) {
  cql_code _rc_ = SQLITE_OK;
  *_result_stmt = NULL;
  cql_error_prepare();

  _rc_ = cql_prepare(_db_, _result_stmt,
    "SELECT line, data "
      "FROM source_input");
  if (_rc_ != SQLITE_OK) { cql_error_trace(); goto cql_cleanup; }
  _rc_ = SQLITE_OK;

cql_cleanup:
  cql_error_report();
  if (_rc_ == SQLITE_OK && !*_result_stmt) _rc_ = cql_no_rows_stmt(_db_, _result_stmt);
  return _rc_;
}
#undef _PROC_
#pragma clang diagnostic pop
