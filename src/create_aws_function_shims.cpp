#include "create_aws_function_shims.hpp"

#include "duckdb/common/exception.hpp"
#include "duckdb/function/function_set.hpp"
#include "duckdb/main/extension/extension_loader.hpp"

namespace duckdb {

namespace {

static constexpr const char *REMOVED_MESSAGE =
    "load_aws_credentials is no longer supported. Use `CREATE SECRET cfg (TYPE S3, PROVIDER credential_chain)` "
    "instead.";

static unique_ptr<FunctionData> LoadAWSCredentialsBind(ClientContext &, TableFunctionBindInput &, vector<LogicalType> &,
                                                       vector<Identifier> &) {
	throw InvalidInputException(REMOVED_MESSAGE);
}

static void LoadAWSCredentialsFunction(ClientContext &, TableFunctionInput &, DataChunk &) {
	throw InvalidInputException(REMOVED_MESSAGE);
}

} // namespace

void CreateAwsFunctionShims::Register(ExtensionLoader &loader) {
	TableFunctionSet function_set("load_aws_credentials");
	auto base_fun = TableFunction("load_aws_credentials", {}, LoadAWSCredentialsFunction, LoadAWSCredentialsBind);
	auto profile_fun = TableFunction("load_aws_credentials", {LogicalTypeId::VARCHAR}, LoadAWSCredentialsFunction,
	                                 LoadAWSCredentialsBind);

	base_fun.named_parameters["set_region"] = LogicalTypeId::BOOLEAN;
	base_fun.named_parameters["redact_secret"] = LogicalTypeId::BOOLEAN;
	profile_fun.named_parameters["set_region"] = LogicalTypeId::BOOLEAN;
	profile_fun.named_parameters["redact_secret"] = LogicalTypeId::BOOLEAN;

	function_set.AddFunction(base_fun);
	function_set.AddFunction(profile_fun);

	loader.RegisterFunction(function_set);
}

} // namespace duckdb
