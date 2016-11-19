// File: resource_provider.cpp Author: Vader Created on 2016. augusztus 26. 10:06

// hpp
#include <libv/ui/resource/resource_provider.hpp>
//// ext
//#include <boost/filesystem/path.hpp>

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

//File_view ProviderIOResource::file(const boost::filesystem::path& filePath) {
//	return doFile(filePath.lexically_normal());
//}
//
//ProviderIOResource::~ProviderIOResource() { }
//
//// -------------------------------------------------------------------------------------------------
//
//UIFont2D_view ProviderGLResource::font(const boost::filesystem::path& filePath) {
//	return doFont(filePath);
//}
//
//UIProgram_view ProviderGLResource::program(const std::string& name) {
//	return doProgram(name);
//}
//
//UIShader_view ProviderGLResource::shader(const boost::filesystem::path& filePath) {
//	return doShader(filePath);
//}
//
//UITexture2D_view ProviderGLResource::texture(const boost::filesystem::path& filePath) {
//	return doTexture(filePath.lexically_normal());
//}
//
//ProviderGLResource::~ProviderGLResource() { }

// -------------------------------------------------------------------------------------------------

//void ProviderProgramDescription::loadProgram(
//		const std::string& name,
//		const boost::filesystem::path& filePathA,
//		const boost::filesystem::path& filePathB) {
//	doLoadProgram(name, filePathA.lexically_normal(), filePathB.lexically_normal());
//}
//
//void ProviderProgramDescription::loadProgram(
//		const std::string& name,
//		const boost::filesystem::path& filePathA,
//		const boost::filesystem::path& filePathB,
//		const boost::filesystem::path& filePathC) {
//	doLoadProgram(name, filePathA.lexically_normal(), filePathB.lexically_normal(), filePathC.lexically_normal());
//}
//
//ProgramDescription ProviderProgramDescription::programDescription(const std::string& name) {
//	return doProgramDescription(name);
//}
//
//ProviderProgramDescription::~ProviderProgramDescription() { }


// -------------------------------------------------------------------------------------------------

} //namespace ui
} //namespace libv
