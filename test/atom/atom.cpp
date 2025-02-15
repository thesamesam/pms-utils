#include "atom/atom_parser.hpp"
#include "x3_util.hpp"

using namespace pms_utils::atom;
namespace parsers = pms_utils::parsers;

int main() {
    const auto ret = try_parse("foo/bar[baz]", parsers::package_dep);

    if (!ret.as_expected) {
        return 1;
    }
}