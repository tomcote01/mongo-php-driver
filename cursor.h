/**
 *  Copyright 2009-2013 10gen, Inc.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
#ifndef MONGO_CURSOR_H
#define MONGO_CURSOR_H 1

void php_mongo_cursor_free(void *object TSRMLS_DC);

/* Tries to read the reply from the database */
int php_mongo_get_reply(mongo_cursor *cursor TSRMLS_DC);

/* Queries the database. Returns SUCCESS or FAILURE. */
int mongo_cursor__do_query(zval *this_ptr, zval *return_value TSRMLS_DC);

/* Reset the cursor to clean up or prepare for another query. Removes cursor
 * from cursor list (and kills it, if necessary).  */
void mongo_util_cursor_reset(mongo_cursor *cursor TSRMLS_DC);

/* Resets cursor and disconnects connection.  Always returns FAILURE (so it can
 * be used by functions returning FAILURE). */
int mongo_util_cursor_failed(mongo_cursor *cursor TSRMLS_DC);

/* If the query should be send to the db or not. The rules are:
 * - db commands should only be sent onces (no retries)
 * - normal queries should be sent up to 5 times
 * This uses exponential backoff with a random seed to avoid flooding a
 * struggling db with retries.  */
int mongo_cursor__should_retry(mongo_cursor *cursor);

/* Kills a cursor on the server */
void php_mongo_kill_cursor(mongo_connection *con, int64_t cursor_id TSRMLS_DC);

/* Set Cursor limit */
void php_mongo_cursor_set_limit(mongo_cursor *cursor, long limit);
/* Forces the bson to zval conversion to use an object for a long */
void php_mongo_cursor_force_long_as_object(mongo_cursor *cursor);
/* Flags the cursor as a command cursor */
void php_mongo_cursor_force_command_cursor(mongo_cursor *cursor);
/* Switch to primary connection */
void php_mongo_cursor_force_primary(mongo_cursor *cursor);

/* Set a Cursor Option */
int php_mongo_cursor_add_option(mongo_cursor *cursor, char *key, zval *value TSRMLS_DC);

PHP_METHOD(MongoCursor, __construct);
PHP_METHOD(MongoCursor, getNext);
PHP_METHOD(MongoCursor, hasNext);
PHP_METHOD(MongoCursor, limit);
PHP_METHOD(MongoCursor, batchSize);
PHP_METHOD(MongoCursor, skip);
PHP_METHOD(MongoCursor, fields);

PHP_METHOD(MongoCursor, setFlag);
PHP_METHOD(MongoCursor, tailable);
PHP_METHOD(MongoCursor, slaveOkay);
PHP_METHOD(MongoCursor, immortal);
PHP_METHOD(MongoCursor, awaitData);
PHP_METHOD(MongoCursor, partial);

PHP_METHOD(MongoCursor, timeout);
PHP_METHOD(MongoCursor, dead);
PHP_METHOD(MongoCursor, snapshot);
PHP_METHOD(MongoCursor, sort);
PHP_METHOD(MongoCursor, hint);

PHP_METHOD(MongoCursor, getReadPreference);
PHP_METHOD(MongoCursor, setReadPreference);

PHP_METHOD(MongoCursor, addOption);
PHP_METHOD(MongoCursor, explain);
PHP_METHOD(MongoCursor, doQuery);
PHP_METHOD(MongoCursor, current);
PHP_METHOD(MongoCursor, key);
PHP_METHOD(MongoCursor, next);
PHP_METHOD(MongoCursor, rewind);
PHP_METHOD(MongoCursor, valid);
PHP_METHOD(MongoCursor, reset);
PHP_METHOD(MongoCursor, count);
PHP_METHOD(MongoCursor, info);

#define PREITERATION_SETUP \
	PHP_MONGO_GET_CURSOR(getThis()); \
	\
	if (cursor->started_iterating) { \
		zend_throw_exception(mongo_ce_CursorException, "cannot modify cursor after beginning iteration.", 0 TSRMLS_CC); \
		return; \
	}

PHP_METHOD(MongoCursorException, getHost);

/* Throw a MongoCursorException with the given code and message.  Uses the
 * server to fill in information about the connection that cause the exception.
 * Does nothing if an exception has already been thrown. */
zval* mongo_cursor_throw(zend_class_entry *exception_ce, mongo_connection *connection, int code TSRMLS_DC, char *format, ...);


#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: fdm=marker
 * vim: noet sw=4 ts=4
 */
