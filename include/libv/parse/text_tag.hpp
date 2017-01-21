// File:   text_tag.hpp Author: Vader Created on 2017. január 18., 0:26

#pragma once

// ext
#include <boost/fusion/adapted/struct.hpp>
#include <boost/spirit/home/x3.hpp>
// std
#include <string>
#include <vector>


// -------------------------------------------------------------------------------------------------

namespace libv {
namespace parse {

struct Tag {
	bool close;
	std::string name;
	std::string value;
};

} // namespace parse
} // namespace libv

BOOST_FUSION_ADAPT_STRUCT(
    ::libv::parse::Tag,
    (bool, close)
    (std::string, name)
    (std::string, value)
)

namespace libv {
namespace parse {

// -------------------------------------------------------------------------------------------------

std::vector<Tag> parseTextTag(const std::string& str) {
	namespace x3 = boost::spirit::x3;

	struct tag_name_ : x3::symbols<std::string> { tag_name_() { add
		("color", "color")
		("font", "font")
		("size", "size")
		("program", "program")
		("test", "test")
		("tag", "tag")
	;}} tag_name;

//	struct special_text_ : x3::symbols<std::string> { special_text_() { add
//		("\\t", "\t")
//		("\\n", "\n")
//	;}} special_text;

	const auto text_wrap = [](auto& ctx) {
		_val(ctx).value = _attr(ctx);
	};

	const auto value = x3::rule<class value, std::string>{} =
			x3::lit('"') >>
				x3::no_skip[*(
					x3::lit('\\') >> x3::char_("\"\\") |
					~x3::char_("\"")
				)]
			>> x3::lit('"') |
			x3::no_skip[*(
				x3::lit('\\') >> x3::char_("]\\") |
				~x3::char_("]")
			)];

	const auto tag = x3::rule<class tag, Tag>{} =
			x3::lit('[') >>
			x3::matches[x3::lit('/')] >>
			x3::no_case[tag_name] >>
			-(x3::lit('=') >> value) >>
			x3::lit(']');

	const auto text = x3::rule<class text, std::string>{} =
			+(
				x3::lit('\\') >> x3::char_("\\[") |
//				special_text |
				!x3::omit[tag] >> x3::char_
			);

	const auto text_tag = x3::rule<class text_tag, Tag>{} =
			text[text_wrap];

//	std::vector<boost::variant<std::string, Tag>> parsed_values;
	std::vector<Tag> parsed_values;

	x3::parse(str.begin(), str.end(), *(text_tag | tag), parsed_values);

	return parsed_values;
}


} // namespace parse
} // namespace libv