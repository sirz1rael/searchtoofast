#include "utils/json_helper.hpp"
#include <boost/json.hpp>

namespace searchtoofast::utils {
void JsonHelper::write_json_pretty(std::ostream& os, const j::value& v, int indent) {
    const int step = 4;
    if (v.is_object()) {
        os << '{' << '\n';
        auto& obj = v.as_object();
        bool first = true;
        for (auto it = obj.begin(); it != obj.end(); ++it) {
            if (!first) os << ',' << '\n';
            first = false;
            os << std::string(indent + step, ' ') << '"' << it->key() << "\": ";
            write_json_pretty(os, it->value(), indent + step);
        }
        os << '\n' << std::string(indent, ' ') << '}';
    } else if (v.is_array()) {
        os << '[' << '\n';
        auto& arr = v.as_array();
        for (std::size_t i = 0; i < arr.size(); ++i) {
            if (i) os << ',' << '\n';
            os << std::string(indent + step, ' ');
            write_json_pretty(os, arr[i], indent + step);
        }
        os << '\n' << std::string(indent, ' ') << ']';
    } else {
        // For scalars and strings use boost::json::serialize which produces
        // the correct JSON representation (including quoted strings).
        os << j::serialize(v);
    }
}

j::value JsonHelper::read_json(std::istream& is, j::error_code& ec) {
    auto v = j::parse(is, ec);
    return v;
}
}; // namespace searchtoofast::utils
