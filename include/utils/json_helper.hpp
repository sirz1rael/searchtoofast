#ifndef JSON_HELPER_HPP
#define JSON_HELPER_HPP

#include <boost/json.hpp>

namespace j = boost::json;

namespace searchtoofast::utils {

class JsonHelper {
public:

static void write_json_pretty(std::ostream& os, const j::value& v, int indent = 0);
static j::value read_json(std::istream& is, j::error_code& ec);

private:


};
};

#endif // JSON_HELPER_HPP
