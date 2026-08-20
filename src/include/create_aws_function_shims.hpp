#pragma once

namespace duckdb {

class ExtensionLoader;

struct CreateAwsFunctionShims {
	static void Register(ExtensionLoader &loader);
};

} // namespace duckdb
