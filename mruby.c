#include <stdlib.h>
#include "redismodule.h" 
#include "mruby.h"
#include "mruby/compile.h"
#include "mruby/string.h"

int MrubyEval_RedisCommand(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
	// アリティのチェック
	if (argc < 2) return RedisModule_WrongArity(ctx);
	size_t len;
	const char *prefix = RedisModule_StringPtrLen(argv[1], &len);
	mrb_state* mrb;
	mrb_value value;
	char *result;

	mrb = mrb_open();

	value = mrb_load_string(mrb, prefix);
	result = (char *)mrb_string_value_ptr(mrb, value);
 	
	if (RedisModule_ReplyWithSimpleString(ctx, result) == REDISMODULE_ERR){
		mrb_close(mrb);
       return REDISMODULE_ERR;
	}
	
	mrb_close(mrb);

	return REDISMODULE_OK;
}

int RedisModule_OnLoad(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
	// モジュールの初期化
	if (RedisModule_Init(ctx, "mruby", 1, REDISMODULE_APIVER_1) == REDISMODULE_ERR)
		return REDISMODULE_ERR;

	if (RedisModule_CreateCommand(ctx, "mruby.eval", MrubyEval_RedisCommand, "write deny-oom random fast", 1, 1, 1) == REDISMODULE_ERR)
		return REDISMODULE_ERR;

	return REDISMODULE_OK;
}
