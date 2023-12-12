// Project: libv, File: app/star/game/scene/scene_credits.cpp

// hpp
#include <star/game/scene/scenes.hpp>
// ext
#include <fmt/format.h>
// libv
#include <libv/ui/component/button.hpp>
#include <libv/ui/component/image.hpp>
#include <libv/ui/component/label.hpp>
#include <libv/ui/component/panel_line.hpp>
#include <libv/ui/component_system/bean.hpp>
#include <libv/ui/component_system/switch_scene.hpp>
// pro
#include <star/game/scene/controls_layout.hpp>

#include <libv/ui/component/scroll_area.hpp>
#include <libv/ui/component/scroll_pane.hpp>


namespace star {

// -------------------------------------------------------------------------------------------------

struct ThirdPartyDescription {
	std::string_view title;
	std::string_view url;
	std::string_view license_short;
	std::string_view license_text;
};

struct PersonnelDescription {
	std::string_view name;
	std::string_view nickname;
	std::string_view position;
};

std::vector<PersonnelDescription> makePersonnelList();
std::vector<ThirdPartyDescription> makeThirdPartyList();

// -------------------------------------------------------------------------------------------------

libv::ui::Component createSceneCredits(libv::Nexus& nexus) {
	auto line = libv::ui::PanelLine::s("credits.main");

	line.add_sa<libv::ui::Label>("settings.title", "Credits");

	auto scroll = line.add_s<libv::ui::ScrollPane>("credits.scroll");
	auto vis_line = libv::ui::PanelLine::s("credits.line");
	scroll.content(vis_line);
	scroll.mode(libv::ui::ScrollMode::vertical);

	{
		vis_line.add_sa<libv::ui::Label>("credits.segment", "Personnel");

		for (const auto& personnel : makePersonnelList()) {
			auto label = vis_line.add_s<libv::ui::Label>("credits.personnel");
			label.text(fmt::format("{} [{}] - {}", personnel.name, personnel.nickname, personnel.position));
		}
	}

	vis_line.add_sa<libv::ui::Label>("credits.segment", "Third-Party libraries and tools");
//	for (int i = 0; i < 10; ++i)
	for (const auto& thirdParty : makeThirdPartyList()) {
		auto panel = vis_line.add_s<libv::ui::PanelLine>("credits.third-party.panel");
		panel.add_s<libv::ui::Image>("credits.third-party.separator");
		panel.add_sa<libv::ui::Label>("credits.third-party.title", std::string(thirdParty.title));
		panel.add_sa<libv::ui::Label>("credits.third-party.url", std::string(thirdParty.url));
		panel.add_sa<libv::ui::Label>("credits.third-party.license_short", "License: " + std::string(thirdParty.license_short));
		panel.add_sa<libv::ui::Label>("credits.third-party.license_text", std::string(thirdParty.license_text));
	}

	auto btn = line.add_sa<libv::ui::Button>("settings.ctrl", "Back");
	btn.event().submit.connect([nexus](libv::ui::Button& source) mutable {
		libv::ui::switchParentScene("main", source, createSceneMainMenu(nexus));
	});

	return line;
}

// -------------------------------------------------------------------------------------------------

std::vector<PersonnelDescription> makePersonnelList() {
	std::vector<PersonnelDescription> list;

	list.emplace_back("Császár Mátyás", "Vader", "Founder");
	list.emplace_back("Császár Mátyás", "Vader", "Technology director");
	list.emplace_back("Császár Mátyás", "Vader", "Administration");
	list.emplace_back("Császár Mátyás", "Vader", "Programmer");

	return list;
}

std::vector<ThirdPartyDescription> makeThirdPartyList() {
	std::vector<ThirdPartyDescription> list;

	// static constexpr ThirdPartyDescription array[] = {
	// 	{
	// 		.title = "assimp - Open Asset Import Library",
	// 		.url = "https://www.assimp.org/",
	// 		.license_short = "3-clause BSD-License (Modified)",
	// 		.license_text =
	// 		"Open Asset Import Library (assimp)\n"
	// 		"\n"
	// 		"Copyright (c) 2006-2021, assimp team\n"
	// 		"All rights reserved.\n"
	// 		"\n"
	// 		"Redistribution and use of this software in source and binary forms, with or without modification, are permitted provided that the following conditions are met:\n"
	// 		"* Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.\n"
	// 		"* Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.\n"
	// 		"* Neither the name of the assimp team, nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission of the assimp team.\n"
	// 		"\n"
	// 		"THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n"
	// 		"\n"
	// 		"******************************************************************************\n"
	// 		"\n"
	// 		"AN EXCEPTION applies to all files in the ./test/models-nonbsd folder. These are 3d models for testing purposes, from various free sources on the internet. They are - unless otherwise stated - copyright of their respective creators, which may impose additional requirements on the use of their work. For any of these models, see <model-name>.source.txt for more legal information. Contact us if you are a copyright holder and believe that we credited you inproperly or if you don't want your files to appear in the repository.\n"
	// 		"\n"
	// 		"******************************************************************************\n"
	// 		"\n"
	// 		"Poly2Tri Copyright (c) 2009-2010, Poly2Tri Contributors\n"
	// 		"http://code.google.com/p/poly2tri/\n"
	// 		"\n"
	// 		"All rights reserved.\n"
	// 		"Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:\n"
	// 		"\n"
	// 		"* Redistributions of source code must retain the above copyright notice this list of conditions and the following disclaimer.\n"
	// 		"* Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.\n"
	// 		"* Neither the name of Poly2Tri nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.\n"
	// 		"\n"
	// 		"THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
	// 	}
	// };

	list.emplace_back(
			"assimp - Open Asset Import Library",

			"https://www.assimp.org/",

			"3-clause BSD-License (Modified)",

			"Open Asset Import Library (assimp)\n"
			"\n"
			"Copyright (c) 2006-2021, assimp team\n"
			"All rights reserved.\n"
			"\n"
			"Redistribution and use of this software in source and binary forms, with or without modification, are permitted provided that the following conditions are met:\n"
			"* Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.\n"
			"* Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.\n"
			"* Neither the name of the assimp team, nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission of the assimp team.\n"
			"\n"
			"THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n"
			"\n"
			"******************************************************************************\n"
			"\n"
			"AN EXCEPTION applies to all files in the ./test/models-nonbsd folder. These are 3d models for testing purposes, from various free sources on the internet. They are - unless otherwise stated - copyright of their respective creators, which may impose additional requirements on the use of their work. For any of these models, see <model-name>.source.txt for more legal information. Contact us if you are a copyright holder and believe that we credited you inproperly or if you don't want your files to appear in the repository.\n"
			"\n"
			"******************************************************************************\n"
			"\n"
			"Poly2Tri Copyright (c) 2009-2010, Poly2Tri Contributors\n"
			"http://code.google.com/p/poly2tri/\n"
			"\n"
			"All rights reserved.\n"
			"Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:\n"
			"\n"
			"* Redistributions of source code must retain the above copyright notice this list of conditions and the following disclaimer.\n"
			"* Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.\n"
			"* Neither the name of Poly2Tri nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.\n"
			"\n"
			"THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
	);

	list.emplace_back(
			"Boost",

			"https://www.boost.org/",

			"Boost Software License 1.0",

			"Boost Software License - Version 1.0 - August 17th, 2003\n"
			"\n"
			"Permission is hereby granted, free of charge, to any person or organization obtaining a copy of the software and accompanying documentation covered by this license (the \"Software\") to use, reproduce, display, distribute, execute, and transmit the Software, and to prepare derivative works of the Software, and to permit third-parties to whom the Software is furnished to do so, all subject to the following:\n"
			"\n"
			"The copyright notices in the Software and this entire statement, including the above license grant, this restriction and the following disclaimer, must be included in all copies of the Software, in whole or in part, and all derivative works of the Software, unless such copies or derivative works are solely in the form of machine-executable object code generated by a source language processor.\n"
			"\n"
			"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE."
	);

	list.emplace_back(
			"Catch2",

			"https://github.com/catchorg/Catch2",

			"Boost Software License 1.0",

			"Boost Software License - Version 1.0 - August 17th, 2003\n"
			"\n"
			"Permission is hereby granted, free of charge, to any person or organization obtaining a copy of the software and accompanying documentation covered by this license (the \"Software\") to use, reproduce, display, distribute, execute, and transmit the Software, and to prepare derivative works of the Software, and to permit third-parties to whom the Software is furnished to do so, all subject to the following:\n"
			"\n"
			"The copyright notices in the Software and this entire statement, including the above license grant, this restriction and the following disclaimer, must be included in all copies of the Software, in whole or in part, and all derivative works of the Software, unless such copies or derivative works are solely in the form of machine-executable object code generated by a source language processor.\n"
			"\n"
			"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE."
	);

	list.emplace_back(
			"Clip Library",

			"https://github.com/dacap/clip",

			"MIT license",

			"Copyright (c) 2015-2022 David Capello\n"
			"\n"
			"Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the \"Software\"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:\n"
			"\n"
			"The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.\n"
			"\n"
			"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE."
	);

	list.emplace_back(
			"CppCoro",

			"https://github.com/andreasbuhr/cppcoro\n"
			"https://github.com/VaderY/cppcoro (Fork)",

			"MIT license",

			"Copyright 2017 Lewis Baker\n"
			"\n"
			"Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the \"Software\"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:\n"
			"\n"
			"The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.\n"
			"\n"
			"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE."
	);

	list.emplace_back(
			"Compile time regular expressions",

			"https://github.com/hanickadot/compile-time-regular-expressions",

			"Apache License 2.0",

			"                                 Apache License\n"
			"                           Version 2.0, January 2004\n"
			"                        http://www.apache.org/licenses/\n"
			"\n"
			"    TERMS AND CONDITIONS FOR USE, REPRODUCTION, AND DISTRIBUTION\n"
			"\n"
			"    1. Definitions.\n"
			"\n"
			"      \"License\" shall mean the terms and conditions for use, reproduction,\n"
			"      and distribution as defined by Sections 1 through 9 of this document.\n"
			"\n"
			"      \"Licensor\" shall mean the copyright owner or entity authorized by\n"
			"      the copyright owner that is granting the License.\n"
			"\n"
			"      \"Legal Entity\" shall mean the union of the acting entity and all\n"
			"      other entities that control, are controlled by, or are under common\n"
			"      control with that entity. For the purposes of this definition,\n"
			"      \"control\" means (i) the power, direct or indirect, to cause the\n"
			"      direction or management of such entity, whether by contract or\n"
			"      otherwise, or (ii) ownership of fifty percent (50%) or more of the\n"
			"      outstanding shares, or (iii) beneficial ownership of such entity.\n"
			"\n"
			"      \"You\" (or \"Your\") shall mean an individual or Legal Entity\n"
			"      exercising permissions granted by this License.\n"
			"\n"
			"      \"Source\" form shall mean the preferred form for making modifications,\n"
			"      including but not limited to software source code, documentation\n"
			"      source, and configuration files.\n"
			"\n"
			"      \"Object\" form shall mean any form resulting from mechanical\n"
			"      transformation or translation of a Source form, including but\n"
			"      not limited to compiled object code, generated documentation,\n"
			"      and conversions to other media types.\n"
			"\n"
			"      \"Work\" shall mean the work of authorship, whether in Source or\n"
			"      Object form, made available under the License, as indicated by a\n"
			"      copyright notice that is included in or attached to the work\n"
			"      (an example is provided in the Appendix below).\n"
			"\n"
			"      \"Derivative Works\" shall mean any work, whether in Source or Object\n"
			"      form, that is based on (or derived from) the Work and for which the\n"
			"      editorial revisions, annotations, elaborations, or other modifications\n"
			"      represent, as a whole, an original work of authorship. For the purposes\n"
			"      of this License, Derivative Works shall not include works that remain\n"
			"      separable from, or merely link (or bind by name) to the interfaces of,\n"
			"      the Work and Derivative Works thereof.\n"
			"\n"
			"      \"Contribution\" shall mean any work of authorship, including\n"
			"      the original version of the Work and any modifications or additions\n"
			"      to that Work or Derivative Works thereof, that is intentionally\n"
			"      submitted to Licensor for inclusion in the Work by the copyright owner\n"
			"      or by an individual or Legal Entity authorized to submit on behalf of\n"
			"      the copyright owner. For the purposes of this definition, \"submitted\"\n"
			"      means any form of electronic, verbal, or written communication sent\n"
			"      to the Licensor or its representatives, including but not limited to\n"
			"      communication on electronic mailing lists, source code control systems,\n"
			"      and issue tracking systems that are managed by, or on behalf of, the\n"
			"      Licensor for the purpose of discussing and improving the Work, but\n"
			"      excluding communication that is conspicuously marked or otherwise\n"
			"      designated in writing by the copyright owner as \"Not a Contribution.\"\n"
			"\n"
			"      \"Contributor\" shall mean Licensor and any individual or Legal Entity\n"
			"      on behalf of whom a Contribution has been received by Licensor and\n"
			"      subsequently incorporated within the Work.\n"
			"\n"
			"    2. Grant of Copyright License. Subject to the terms and conditions of\n"
			"      this License, each Contributor hereby grants to You a perpetual,\n"
			"      worldwide, non-exclusive, no-charge, royalty-free, irrevocable\n"
			"      copyright license to reproduce, prepare Derivative Works of,\n"
			"      publicly display, publicly perform, sublicense, and distribute the\n"
			"      Work and such Derivative Works in Source or Object form.\n"
			"\n"
			"    3. Grant of Patent License. Subject to the terms and conditions of\n"
			"      this License, each Contributor hereby grants to You a perpetual,\n"
			"      worldwide, non-exclusive, no-charge, royalty-free, irrevocable\n"
			"      (except as stated in this section) patent license to make, have made,\n"
			"      use, offer to sell, sell, import, and otherwise transfer the Work,\n"
			"      where such license applies only to those patent claims licensable\n"
			"      by such Contributor that are necessarily infringed by their\n"
			"      Contribution(s) alone or by combination of their Contribution(s)\n"
			"      with the Work to which such Contribution(s) was submitted. If You\n"
			"      institute patent litigation against any entity (including a\n"
			"      cross-claim or counterclaim in a lawsuit) alleging that the Work\n"
			"      or a Contribution incorporated within the Work constitutes direct\n"
			"      or contributory patent infringement, then any patent licenses\n"
			"      granted to You under this License for that Work shall terminate\n"
			"      as of the date such litigation is filed.\n"
			"\n"
			"    4. Redistribution. You may reproduce and distribute copies of the\n"
			"      Work or Derivative Works thereof in any medium, with or without\n"
			"      modifications, and in Source or Object form, provided that You\n"
			"      meet the following conditions:\n"
			"\n"
			"      (a) You must give any other recipients of the Work or\n"
			"          Derivative Works a copy of this License; and\n"
			"\n"
			"      (b) You must cause any modified files to carry prominent notices\n"
			"          stating that You changed the files; and\n"
			"\n"
			"      (c) You must retain, in the Source form of any Derivative Works\n"
			"          that You distribute, all copyright, patent, trademark, and\n"
			"          attribution notices from the Source form of the Work,\n"
			"          excluding those notices that do not pertain to any part of\n"
			"          the Derivative Works; and\n"
			"\n"
			"      (d) If the Work includes a \"NOTICE\" text file as part of its\n"
			"          distribution, then any Derivative Works that You distribute must\n"
			"          include a readable copy of the attribution notices contained\n"
			"          within such NOTICE file, excluding those notices that do not\n"
			"          pertain to any part of the Derivative Works, in at least one\n"
			"          of the following places: within a NOTICE text file distributed\n"
			"          as part of the Derivative Works; within the Source form or\n"
			"          documentation, if provided along with the Derivative Works; or,\n"
			"          within a display generated by the Derivative Works, if and\n"
			"          wherever such third-party notices normally appear. The contents\n"
			"          of the NOTICE file are for informational purposes only and\n"
			"          do not modify the License. You may add Your own attribution\n"
			"          notices within Derivative Works that You distribute, alongside\n"
			"          or as an addendum to the NOTICE text from the Work, provided\n"
			"          that such additional attribution notices cannot be construed\n"
			"          as modifying the License.\n"
			"\n"
			"      You may add Your own copyright statement to Your modifications and\n"
			"      may provide additional or different license terms and conditions\n"
			"      for use, reproduction, or distribution of Your modifications, or\n"
			"      for any such Derivative Works as a whole, provided Your use,\n"
			"      reproduction, and distribution of the Work otherwise complies with\n"
			"      the conditions stated in this License.\n"
			"\n"
			"    5. Submission of Contributions. Unless You explicitly state otherwise,\n"
			"      any Contribution intentionally submitted for inclusion in the Work\n"
			"      by You to the Licensor shall be under the terms and conditions of\n"
			"      this License, without any additional terms or conditions.\n"
			"      Notwithstanding the above, nothing herein shall supersede or modify\n"
			"      the terms of any separate license agreement you may have executed\n"
			"      with Licensor regarding such Contributions.\n"
			"\n"
			"    6. Trademarks. This License does not grant permission to use the trade\n"
			"      names, trademarks, service marks, or product names of the Licensor,\n"
			"      except as required for reasonable and customary use in describing the\n"
			"      origin of the Work and reproducing the content of the NOTICE file.\n"
			"\n"
			"    7. Disclaimer of Warranty. Unless required by applicable law or\n"
			"      agreed to in writing, Licensor provides the Work (and each\n"
			"      Contributor provides its Contributions) on an \"AS IS\" BASIS,\n"
			"      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or\n"
			"      implied, including, without limitation, any warranties or conditions\n"
			"      of TITLE, NON-INFRINGEMENT, MERCHANTABILITY, or FITNESS FOR A\n"
			"      PARTICULAR PURPOSE. You are solely responsible for determining the\n"
			"      appropriateness of using or redistributing the Work and assume any\n"
			"      risks associated with Your exercise of permissions under this License.\n"
			"\n"
			"    8. Limitation of Liability. In no event and under no legal theory,\n"
			"      whether in tort (including negligence), contract, or otherwise,\n"
			"      unless required by applicable law (such as deliberate and grossly\n"
			"      negligent acts) or agreed to in writing, shall any Contributor be\n"
			"      liable to You for damages, including any direct, indirect, special,\n"
			"      incidental, or consequential damages of any character arising as a\n"
			"      result of this License or out of the use or inability to use the\n"
			"      Work (including but not limited to damages for loss of goodwill,\n"
			"      work stoppage, computer failure or malfunction, or any and all\n"
			"      other commercial damages or losses), even if such Contributor\n"
			"      has been advised of the possibility of such damages.\n"
			"\n"
			"    9. Accepting Warranty or Additional Liability. While redistributing\n"
			"      the Work or Derivative Works thereof, You may choose to offer,\n"
			"      and charge a fee for, acceptance of support, warranty, indemnity,\n"
			"      or other liability obligations and/or rights consistent with this\n"
			"      License. However, in accepting such obligations, You may act only\n"
			"      on Your own behalf and on Your sole responsibility, not on behalf\n"
			"      of any other Contributor, and only if You agree to indemnify,\n"
			"      defend, and hold each Contributor harmless for any liability\n"
			"      incurred by, or claims asserted against, such Contributor by reason\n"
			"      of your accepting any such warranty or additional liability.\n"
			"\n"
			"    END OF TERMS AND CONDITIONS\n"
			"\n"
			"    APPENDIX: How to apply the Apache License to your work.\n"
			"\n"
			"      To apply the Apache License to your work, attach the following\n"
			"      boilerplate notice, with the fields enclosed by brackets \"[]\"\n"
			"      replaced with your own identifying information. (Don't include\n"
			"      the brackets!)  The text should be enclosed in the appropriate\n"
			"      comment syntax for the file format. We also recommend that a\n"
			"      file or class name and description of purpose be included on the\n"
			"      same \"printed page\" as the copyright notice for easier\n"
			"      identification within third-party archives.\n"
			"\n"
			"    Copyright [yyyy] [name of copyright owner]\n"
			"\n"
			"    Licensed under the Apache License, Version 2.0 (the \"License\");\n"
			"    you may not use this file except in compliance with the License.\n"
			"    You may obtain a copy of the License at\n"
			"\n"
			"       http://www.apache.org/licenses/LICENSE-2.0\n"
			"\n"
			"    Unless required by applicable law or agreed to in writing, software\n"
			"    distributed under the License is distributed on an \"AS IS\" BASIS,\n"
			"    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n"
			"    See the License for the specific language governing permissions and\n"
			"    limitations under the License.\n"
			"\n"
			"\n"
			"--- LLVM Exceptions to the Apache 2.0 License ----\n"
			"\n"
			"As an exception, if, as a result of your compiling your source code, portions\n"
			"of this Software are embedded into an Object form of such source code, you\n"
			"may redistribute such embedded portions in such Object form without complying\n"
			"with the conditions of Sections 4(a), 4(b) and 4(d) of the License.\n"
			"\n"
			"In addition, if you combine or link compiled forms of this Software with\n"
			"software that is licensed under the GPLv2 (\"Combined Software\") and if a\n"
			"court of competent jurisdiction determines that the patent provision (Section\n"
			"3), the indemnity provision (Section 9) or other Section of the License\n"
			"conflicts with the conditions of the GPLv2, you may retroactively and\n"
			"prospectively choose to deem waived or otherwise exclude such Section(s) of\n"
			"the License, but only in their entirety and only with respect to the Combined\n"
			"Software."
	);

	list.emplace_back(
			"Date",

			"https://howardhinnant.github.io/date/date.html",

			"MIT license",

			"The source code in this project is released using the MIT License. There is no global license for the project because each file is licensed individually with different author names and/or dates.\n"
			"\n"
			"If you contribute to this project, please add your name to the license of each file you modify.  If you have already contributed to this project and forgot to add your name to the license, please feel free to submit a new P/R to add your name to the license in each file you modified.\n"
			"\n"
			"For convenience, here is a copy of the MIT license found in each file except without author names or dates:\n"
			"\n"
			"The MIT License (MIT)\n"
			"\n"
			"Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the \"Software\"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:\n"
			"\n"
			"The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.\n"
			"\n"
			"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE."
	);

	list.emplace_back(
			"HDiffPatch",

			"https://github.com/sisong/HDiffPatch\n"
			"https://github.com/VaderY/HDiffPatch (Fork)",

			"MIT license",

			"HDiffPatch\n"
			"Copyright (c) 2012-2022 housisong\n"
			"\n"
			"Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the \"Software\"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:\n"
			"\n"
			"The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.\n"
			"\n"
			"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.\n"
			"\n"
			"----------------------------------------------------------------------------------\n"
			"\n"
			"libdivsufsort\n"
			"Copyright (c) 2003-2008 Yuta Mori All Rights Reserved.\n"
			"\n"
			"Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the \"Software\"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:\n"
			"\n"
			"The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.\n"
			"\n"
			"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE."
	);

	list.emplace_back(
			"Entropia File System Watcher",

			"https://github.com/SpartanJ/efsw",

			"MIT license",

			"Copyright (c) 2020 Martín Lucas Golini\n"
			"\n"
			"Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the \"Software\"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:\n"
			"\n"
			"The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.\n"
			"\n"
			"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.\n"
			"\n"
			"This software is a fork of the \"simplefilewatcher\" by James Wynn (james@jameswynn.com)\n"
			"http://code.google.com/p/simplefilewatcher/ also MIT licensed."
	);

	list.emplace_back(
			"{fmt}",

			"https://fmt.dev",

			"MIT license (Modified)",

			"Copyright (c) 2012 - present, Victor Zverovich\n"
			"\n"
			"Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the \"Software\"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:\n"
			"\n"
			"The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.\n"
			"\n"
			"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.\n"
			"\n"
			"--- Optional exception to the license ---\n"
			"\n"
			"As an exception, if, as a result of your compiling your source code, portions of this Software are embedded into a machine-executable object form of such source code, you may redistribute such embedded portions in such object form without including the above copyright and permission notices."
	);

	list.emplace_back(
			"FreeType",

			"http://freetype.org/",

			"FreeType License",

			"The FreeType Project LICENSE\n"
			"----------------------------\n"
			"\n"
			"2006-Jan-27\n"
			"\n"
			"Copyright 1996-2002, 2006 by\n"
			"David Turner, Robert Wilhelm, and Werner Lemberg\n"
			"\n"
			"Introduction\n"
			"============\n"
			"The FreeType Project is distributed in several archive packages; some of them may contain, in addition to the FreeType font engine, various tools and contributions which rely on, or relate to, the FreeType Project.\n"
			"\n"
			"This license applies to all files found in such packages, and which do not fall under their own explicit license. The license affects thus the FreeType font engine, the test programs, documentation and makefiles, at the very least.\n"
			"\n"
			"This license was inspired by the BSD, Artistic, and IJG (Independent JPEG Group) licenses, which all encourage inclusion and use of free software in commercial and freeware products alike. As a consequence, its main points are that:\n"
			"  o We don't promise that this software works. However, we will be interested in any kind of bug reports. (`as is' distribution)\n"
			"  o You can use this software for whatever you want, in parts or full form, without having to pay us. (`royalty-free' usage)\n"
			"  o You may not pretend that you wrote this software. If you use it, or only parts of it, in a program, you must acknowledge somewhere in your documentation that you have used the FreeType code. (`credits')\n"
			"\n"
			"We specifically permit and encourage the inclusion of this software, with or without modifications, in commercial products. We disclaim all warranties covering The FreeType Project and assume no liability related to The FreeType Project.\n"
			"\n"
			"Finally, many people asked us for a preferred form for a credit/disclaimer to use in compliance with this license. We thus encourage you to use the following text:\n"
			"    \"\"\"\n"
			"    Portions of this software are copyright © <year> The FreeType Project (www.freetype.org). All rights reserved.\n"
			"    \"\"\"\n"
			"\n"
			"Please replace <year> with the value from the FreeType version you actually use.\n"
			"\n"
			"Legal Terms\n"
			"===========\n"
			"0. Definitions\n"
			"--------------\n"
			"Throughout this license, the terms `package', `FreeType Project', and `FreeType archive' refer to the set of files originally distributed by the authors (David Turner, Robert Wilhelm, and Werner Lemberg) as the `FreeType Project', be they named as alpha, beta or final release.\n"
			"\n"
			"`You' refers to the licensee, or person using the project, where `using' is a generic term including compiling the project's source code as well as linking it to form a `program' or `executable'. This program is referred to as `a program using the FreeType engine'.\n"
			"\n"
			"This license applies to all files distributed in the original FreeType Project, including all source code, binaries and documentation, unless otherwise stated in the file in its original, unmodified form as distributed in the original archive. If you are unsure whether or not a particular file is covered by this license, you must contact us to verify this.\n"
			"\n"
			"The FreeType Project is copyright (C) 1996-2000 by David Turner, Robert Wilhelm, and Werner Lemberg. All rights reserved except as specified below.\n"
			"\n"
			"1. No Warranty\n"
			"--------------\n"
			"THE FREETYPE PROJECT IS PROVIDED `AS IS' WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL ANY OF THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY DAMAGES CAUSED BY THE USE OR THE INABILITY TO USE, OF THE FREETYPE PROJECT.\n"
			"\n"
			"2. Redistribution\n"
			"-----------------\n"
			"This license grants a worldwide, royalty-free, perpetual and irrevocable right and license to use, execute, perform, compile, display, copy, create derivative works of, distribute and sublicense the FreeType Project (in both source and object code forms) and derivative works thereof for any purpose; and to authorize others to exercise some or all of the rights granted herein, subject to the following conditions:\n"
			"  o Redistribution of source code must retain this license file (`FTL.TXT') unaltered; any additions, deletions or changes to the original files must be clearly indicated in accompanying documentation. The copyright notices of the unaltered, original files must be preserved in all copies of source files.\n"
			"  o Redistribution in binary form must provide a disclaimer that states that the software is based in part of the work of the FreeType Team, in the distribution documentation. We also encourage you to put an URL to the FreeType web page in your documentation, though this isn't mandatory.\n"
			"\n"
			"These conditions apply to any software derived from or based on the FreeType Project, not just the unmodified files. If you use our work, you must acknowledge us. However, no fee need be paid to us.\n"
			"\n"
			"3. Advertising\n"
			"--------------\n"
			"Neither the FreeType authors and contributors nor you shall use the name of the other for commercial, advertising, or promotional purposes without specific prior written permission.\n"
			"\n"
			"We suggest, but do not require, that you use one or more of the following phrases to refer to this software in your documentation or advertising materials: `FreeType Project', `FreeType Engine', `FreeType library', or `FreeType Distribution'.\n"
			"\n"
			"As you have not signed this license, you are not required to accept it. However, as the FreeType Project is copyrighted material, only this license, or another one contracted with the authors, grants you the right to use, distribute, and modify it. Therefore, by using, distributing, or modifying the FreeType Project, you indicate that you understand and accept all the terms of this license.\n"
			"\n"
			"4. Contacts\n"
			"-----------\n"
			"There are two mailing lists related to FreeType:\n"
			"  o freetype@nongnu.org\n"
			"    Discusses general use and applications of FreeType, as well as future and wanted additions to the library and distribution. If you are looking for support, start in this list if you haven't found anything to help you in the documentation.\n"
			"  o freetype-devel@nongnu.org\n"
			"    Discusses bugs, as well as engine internals, design issues,\n"
			"    specific licenses, porting, etc.\n"
			"\n"
			"Our home page can be found at\n"
			"    https://www.freetype.org"
	);

	list.emplace_back(
			"function2",

			"https://naios.github.io/function2/",

			"Boost Software License 1.0",

			"Boost Software License - Version 1.0 - August 17th, 2003\n"
			"\n"
			"Permission is hereby granted, free of charge, to any person or organization obtaining a copy of the software and accompanying documentation covered by this license (the \"Software\") to use, reproduce, display, distribute, execute, and transmit the Software, and to prepare derivative works of the Software, and to permit third-parties to whom the Software is furnished to do so, all subject to the following:\n"
			"\n"
			"The copyright notices in the Software and this entire statement, including the above license grant, this restriction and the following disclaimer, must be included in all copies of the Software, in whole or in part, and all derivative works of the Software, unless such copies or derivative works are solely in the form of machine-executable object code generated by a source language processor.\n"
			"\n"
			"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE."
	);

	list.emplace_back(
			"GLEW - The OpenGL Extension Wrangler Library",

			"https://glew.sourceforge.net/",

			"Khronos License (MIT)",

			"Copyright (c) 2007 The Khronos Group Inc.\n"
			"\n"
			"Permission is hereby granted, free of charge, to any person obtaining a copy of this software and/or associated documentation files (the \"Materials\"), to deal in the Materials without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Materials, and to permit persons to whom the Materials are furnished to do so, subject to the following conditions:\n"
			"\n"
			"The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Materials.\n"
			"\n"
			"THE MATERIALS ARE PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS."
	);

	list.emplace_back(
			"GLFW",

			"https://www.glfw.org/",

			"zlib License",

			"Copyright (c) 2002-2006 Marcus Geelnard\n"
			"Copyright (c) 2006-2019 Camilla Löwy\n"
			"\n"
			"This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.\n"
			"\n"
			"Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:\n"
			"    1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.\n"
			"    2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.\n"
			"    3. This notice may not be removed or altered from any source distribution."
	);

	list.emplace_back(
			"GLI",

			"https://github.com/g-truc/gli",

			"MIT License",

			"Copyright (c) 2010 - 2020 G-Truc Creation\n"
			"\n"
			"Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the \"Software\"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:\n"
			"\n"
			"The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.\n"
			"\n"
			"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE."
	);

	list.emplace_back(
			"GLM",

			"https://github.com/g-truc/glm",

			"MIT License",

			"Copyright (c) 2005 - G-Truc Creation\n"
			"\n"
			"Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the \"Software\"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:\n"
			"\n"
			"The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.\n"
			"\n"
			"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE."
	);

	list.emplace_back(
			"OpenSSL",

			"https://www.openssl.org/",

			"Apache License 2.0",

			"                                 Apache License\n"
			"                           Version 2.0, January 2004\n"
			"                        https://www.apache.org/licenses/\n"
			"\n"
			"   TERMS AND CONDITIONS FOR USE, REPRODUCTION, AND DISTRIBUTION\n"
			"\n"
			"   1. Definitions.\n"
			"\n"
			"      \"License\" shall mean the terms and conditions for use, reproduction,\n"
			"      and distribution as defined by Sections 1 through 9 of this document.\n"
			"\n"
			"      \"Licensor\" shall mean the copyright owner or entity authorized by\n"
			"      the copyright owner that is granting the License.\n"
			"\n"
			"      \"Legal Entity\" shall mean the union of the acting entity and all\n"
			"      other entities that control, are controlled by, or are under common\n"
			"      control with that entity. For the purposes of this definition,\n"
			"      \"control\" means (i) the power, direct or indirect, to cause the\n"
			"      direction or management of such entity, whether by contract or\n"
			"      otherwise, or (ii) ownership of fifty percent (50%) or more of the\n"
			"      outstanding shares, or (iii) beneficial ownership of such entity.\n"
			"\n"
			"      \"You\" (or \"Your\") shall mean an individual or Legal Entity\n"
			"      exercising permissions granted by this License.\n"
			"\n"
			"      \"Source\" form shall mean the preferred form for making modifications,\n"
			"      including but not limited to software source code, documentation\n"
			"      source, and configuration files.\n"
			"\n"
			"      \"Object\" form shall mean any form resulting from mechanical\n"
			"      transformation or translation of a Source form, including but\n"
			"      not limited to compiled object code, generated documentation,\n"
			"      and conversions to other media types.\n"
			"\n"
			"      \"Work\" shall mean the work of authorship, whether in Source or\n"
			"      Object form, made available under the License, as indicated by a\n"
			"      copyright notice that is included in or attached to the work\n"
			"      (an example is provided in the Appendix below).\n"
			"\n"
			"      \"Derivative Works\" shall mean any work, whether in Source or Object\n"
			"      form, that is based on (or derived from) the Work and for which the\n"
			"      editorial revisions, annotations, elaborations, or other modifications\n"
			"      represent, as a whole, an original work of authorship. For the purposes\n"
			"      of this License, Derivative Works shall not include works that remain\n"
			"      separable from, or merely link (or bind by name) to the interfaces of,\n"
			"      the Work and Derivative Works thereof.\n"
			"\n"
			"      \"Contribution\" shall mean any work of authorship, including\n"
			"      the original version of the Work and any modifications or additions\n"
			"      to that Work or Derivative Works thereof, that is intentionally\n"
			"      submitted to Licensor for inclusion in the Work by the copyright owner\n"
			"      or by an individual or Legal Entity authorized to submit on behalf of\n"
			"      the copyright owner. For the purposes of this definition, \"submitted\"\n"
			"      means any form of electronic, verbal, or written communication sent\n"
			"      to the Licensor or its representatives, including but not limited to\n"
			"      communication on electronic mailing lists, source code control systems,\n"
			"      and issue tracking systems that are managed by, or on behalf of, the\n"
			"      Licensor for the purpose of discussing and improving the Work, but\n"
			"      excluding communication that is conspicuously marked or otherwise\n"
			"      designated in writing by the copyright owner as \"Not a Contribution.\"\n"
			"\n"
			"      \"Contributor\" shall mean Licensor and any individual or Legal Entity\n"
			"      on behalf of whom a Contribution has been received by Licensor and\n"
			"      subsequently incorporated within the Work.\n"
			"\n"
			"   2. Grant of Copyright License. Subject to the terms and conditions of\n"
			"      this License, each Contributor hereby grants to You a perpetual,\n"
			"      worldwide, non-exclusive, no-charge, royalty-free, irrevocable\n"
			"      copyright license to reproduce, prepare Derivative Works of,\n"
			"      publicly display, publicly perform, sublicense, and distribute the\n"
			"      Work and such Derivative Works in Source or Object form.\n"
			"\n"
			"   3. Grant of Patent License. Subject to the terms and conditions of\n"
			"      this License, each Contributor hereby grants to You a perpetual,\n"
			"      worldwide, non-exclusive, no-charge, royalty-free, irrevocable\n"
			"      (except as stated in this section) patent license to make, have made,\n"
			"      use, offer to sell, sell, import, and otherwise transfer the Work,\n"
			"      where such license applies only to those patent claims licensable\n"
			"      by such Contributor that are necessarily infringed by their\n"
			"      Contribution(s) alone or by combination of their Contribution(s)\n"
			"      with the Work to which such Contribution(s) was submitted. If You\n"
			"      institute patent litigation against any entity (including a\n"
			"      cross-claim or counterclaim in a lawsuit) alleging that the Work\n"
			"      or a Contribution incorporated within the Work constitutes direct\n"
			"      or contributory patent infringement, then any patent licenses\n"
			"      granted to You under this License for that Work shall terminate\n"
			"      as of the date such litigation is filed.\n"
			"\n"
			"   4. Redistribution. You may reproduce and distribute copies of the\n"
			"      Work or Derivative Works thereof in any medium, with or without\n"
			"      modifications, and in Source or Object form, provided that You\n"
			"      meet the following conditions:\n"
			"\n"
			"      (a) You must give any other recipients of the Work or\n"
			"          Derivative Works a copy of this License; and\n"
			"\n"
			"      (b) You must cause any modified files to carry prominent notices\n"
			"          stating that You changed the files; and\n"
			"\n"
			"      (c) You must retain, in the Source form of any Derivative Works\n"
			"          that You distribute, all copyright, patent, trademark, and\n"
			"          attribution notices from the Source form of the Work,\n"
			"          excluding those notices that do not pertain to any part of\n"
			"          the Derivative Works; and\n"
			"\n"
			"      (d) If the Work includes a \"NOTICE\" text file as part of its\n"
			"          distribution, then any Derivative Works that You distribute must\n"
			"          include a readable copy of the attribution notices contained\n"
			"          within such NOTICE file, excluding those notices that do not\n"
			"          pertain to any part of the Derivative Works, in at least one\n"
			"          of the following places: within a NOTICE text file distributed\n"
			"          as part of the Derivative Works; within the Source form or\n"
			"          documentation, if provided along with the Derivative Works; or,\n"
			"          within a display generated by the Derivative Works, if and\n"
			"          wherever such third-party notices normally appear. The contents\n"
			"          of the NOTICE file are for informational purposes only and\n"
			"          do not modify the License. You may add Your own attribution\n"
			"          notices within Derivative Works that You distribute, alongside\n"
			"          or as an addendum to the NOTICE text from the Work, provided\n"
			"          that such additional attribution notices cannot be construed\n"
			"          as modifying the License.\n"
			"\n"
			"      You may add Your own copyright statement to Your modifications and\n"
			"      may provide additional or different license terms and conditions\n"
			"      for use, reproduction, or distribution of Your modifications, or\n"
			"      for any such Derivative Works as a whole, provided Your use,\n"
			"      reproduction, and distribution of the Work otherwise complies with\n"
			"      the conditions stated in this License.\n"
			"\n"
			"   5. Submission of Contributions. Unless You explicitly state otherwise,\n"
			"      any Contribution intentionally submitted for inclusion in the Work\n"
			"      by You to the Licensor shall be under the terms and conditions of\n"
			"      this License, without any additional terms or conditions.\n"
			"      Notwithstanding the above, nothing herein shall supersede or modify\n"
			"      the terms of any separate license agreement you may have executed\n"
			"      with Licensor regarding such Contributions.\n"
			"\n"
			"   6. Trademarks. This License does not grant permission to use the trade\n"
			"      names, trademarks, service marks, or product names of the Licensor,\n"
			"      except as required for reasonable and customary use in describing the\n"
			"      origin of the Work and reproducing the content of the NOTICE file.\n"
			"\n"
			"   7. Disclaimer of Warranty. Unless required by applicable law or\n"
			"      agreed to in writing, Licensor provides the Work (and each\n"
			"      Contributor provides its Contributions) on an \"AS IS\" BASIS,\n"
			"      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or\n"
			"      implied, including, without limitation, any warranties or conditions\n"
			"      of TITLE, NON-INFRINGEMENT, MERCHANTABILITY, or FITNESS FOR A\n"
			"      PARTICULAR PURPOSE. You are solely responsible for determining the\n"
			"      appropriateness of using or redistributing the Work and assume any\n"
			"      risks associated with Your exercise of permissions under this License.\n"
			"\n"
			"   8. Limitation of Liability. In no event and under no legal theory,\n"
			"      whether in tort (including negligence), contract, or otherwise,\n"
			"      unless required by applicable law (such as deliberate and grossly\n"
			"      negligent acts) or agreed to in writing, shall any Contributor be\n"
			"      liable to You for damages, including any direct, indirect, special,\n"
			"      incidental, or consequential damages of any character arising as a\n"
			"      result of this License or out of the use or inability to use the\n"
			"      Work (including but not limited to damages for loss of goodwill,\n"
			"      work stoppage, computer failure or malfunction, or any and all\n"
			"      other commercial damages or losses), even if such Contributor\n"
			"      has been advised of the possibility of such damages.\n"
			"\n"
			"   9. Accepting Warranty or Additional Liability. While redistributing\n"
			"      the Work or Derivative Works thereof, You may choose to offer,\n"
			"      and charge a fee for, acceptance of support, warranty, indemnity,\n"
			"      or other liability obligations and/or rights consistent with this\n"
			"      License. However, in accepting such obligations, You may act only\n"
			"      on Your own behalf and on Your sole responsibility, not on behalf\n"
			"      of any other Contributor, and only if You agree to indemnify,\n"
			"      defend, and hold each Contributor harmless for any liability\n"
			"      incurred by, or claims asserted against, such Contributor by reason\n"
			"      of your accepting any such warranty or additional liability.\n"
			"\n"
			"   END OF TERMS AND CONDITIONS"
	);

	list.emplace_back(
			"Immer",

			"https://sinusoid.es/immer/",

			"Boost Software License 1.0",

			"Boost Software License - Version 1.0 - August 17th, 2003\n"
			"\n"
			"Permission is hereby granted, free of charge, to any person or organization obtaining a copy of the software and accompanying documentation covered by this license (the \"Software\") to use, reproduce, display, distribute, execute, and transmit the Software, and to prepare derivative works of the Software, and to permit third-parties to whom the Software is furnished to do so, all subject to the following:\n"
			"\n"
			"The copyright notices in the Software and this entire statement, including the above license grant, this restriction and the following disclaimer, must be included in all copies of the Software, in whole or in part, and all derivative works of the Software, unless such copies or derivative works are solely in the form of machine-executable object code generated by a source language processor.\n"
			"\n"
			"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE."
	);

	list.emplace_back(
			"libbacktrace",

			"https://github.com/ianlancetaylor/libbacktrace",

			"BSD license",

			"Copyright (C) 2012-2016 Free Software Foundation, Inc.\n"
			"\n"
			"Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:\n"
			"    (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.\n"
			"    (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.\n"
			"    (3) The name of the author may not be used to endorse or promote products derived from this software without specific prior written permission.\n"
			"\n"
			"THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
	);

	list.emplace_back(
			"Lua",

			"https://www.lua.org",

			"MIT license",

			"Copyright © 1994–2021 Lua.org, PUC-Rio.\n"
			"\n"
			"Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the \"Software\"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:\n"
			"\n"
			"The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.\n"
			"\n"
			"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE."
	);

	list.emplace_back(
			"plf::colony",

			"https://plflib.org/colony.htm",

			"zlib License",

			"(C) 2019 mattreecebentley\n"
			"\n"
			"This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.\n"
			"\n"
			"Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:\n"
			"    1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.\n"
			"    2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.\n"
			"    3. This notice may not be removed or altered from any source distribution."
	);

	list.emplace_back(
			"Prometheus Client Library for Modern C++",

			"https://github.com/jupp0r/prometheus-cpp",

			"MIT License",

			"Copyright (c) 2016-2021 Jupp Mueller\n"
			"Copyright (c) 2017-2022 Gregor Jasny\n"
			"\n"
			"And many contributors, see\n"
			"https://github.com/jupp0r/prometheus-cpp/graphs/contributors\n"
			"\n"
			"Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the \"Software\"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:\n"
			"\n"
			"The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.\n"
			"\n"
			"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE."
	);

	list.emplace_back(
			"range-v3",

			"https://github.com/ericniebler/range-v3",

			"Custom",

			"========================================================\n"
			"Boost Software License - Version 1.0 - August 17th, 2003\n"
			"========================================================\n"
			"\n"
			"Permission is hereby granted, free of charge, to any person or organization obtaining a copy of the software and accompanying documentation covered by this license (the \"Software\") to use, reproduce, display, distribute, execute, and transmit the Software, and to prepare derivative works of the Software, and to permit third-parties to whom the Software is furnished to do so, all subject to the following:\n"
			"\n"
			"The copyright notices in the Software and this entire statement, including the above license grant, this restriction and the following disclaimer, must be included in all copies of the Software, in whole or in part, and all derivative works of the Software, unless such copies or derivative works are solely in the form of machine-executable object code generated by a source language processor.\n"
			"\n"
			"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.\n"
			"\n"
			"==============================================================================\n"
			"libc++ License\n"
			"==============================================================================\n"
			"\n"
			"The libc++ library is dual licensed under both the University of Illinois \"BSD-Like\" license and the MIT license.  As a user of this code you may choose to use it under either license.  As a contributor, you agree to allow your code to be used under both.\n"
			"\n"
			"Full text of the relevant licenses is included below.\n"
			"\n"
			"==============================================================================\n"
			"\n"
			"University of Illinois/NCSA\n"
			"Open Source License\n"
			"\n"
			"Copyright (c) 2009-2014 by the contributors listed in CREDITS.TXT\n"
			"http://llvm.org/svn/llvm-project/libcxx/trunk/CREDITS.TXT\n"
			"\n"
			"All rights reserved.\n"
			"\n"
			"Developed by:\n"
			"    LLVM Team\n"
			"    University of Illinois at Urbana-Champaign\n"
			"    http://llvm.org\n"
			"\n"
			"Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the \"Software\"), to deal with the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:\n"
			"    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimers.\n"
			"    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimers in the documentation and/or other materials provided with the distribution.\n"
			"    * Neither the names of the LLVM Team, University of Illinois at Urbana-Champaign, nor the names of its contributors may be used to endorse or promote products derived from this Software without specific prior written permission.\n"
			"\n"
			"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS WITH THE SOFTWARE.\n"
			"\n"
			"==============================================================================\n"
			"\n"
			"Copyright (c) 2009-2014 by the contributors listed in CREDITS.TXT\n"
			"  http://llvm.org/svn/llvm-project/libcxx/trunk/CREDITS.TXT\n"
			"\n"
			"Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the \"Software\"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:\n"
			"\n"
			"The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.\n"
			"\n"
			"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.\n"
			"\n"
			"==============================================================================\n"
			"Stepanov and McJones, \"Elements of Programming\" license\n"
			"==============================================================================\n"
			"\n"
			"Copyright (c) 2009 Alexander Stepanov and Paul McJones\n"
			"\n"
			"Permission to use, copy, modify, distribute and sell this software and its documentation for any purpose is hereby granted without fee, provided that the above copyright notice appear in all copies and that both that copyright notice and this permission notice appear in supporting documentation. The authors make no representations about the suitability of this software for any purpose. It is provided \"as is\" without express or implied warranty.\n"
			"\n"
			"Algorithms from\n"
			"Elements of Programming\n"
			"by Alexander Stepanov and Paul McJones\n"
			"Addison-Wesley Professional, 2009\n"
			"\n"
			"==============================================================================\n"
			"SGI C++ Standard Template Library license\n"
			"==============================================================================\n"
			"\n"
			"Copyright (c) 1994\n"
			"Hewlett-Packard Company\n"
			"\n"
			"Permission to use, copy, modify, distribute and sell this software and its documentation for any purpose is hereby granted without fee, provided that the above copyright notice appear in all copies and that both that copyright notice and this permission notice appear in supporting documentation. Hewlett-Packard Company makes no representations about the suitability of this software for any purpose. It is provided \"as is\" without express or implied warranty.\n"
			"\n"
			"Copyright (c) 1996\n"
			"Silicon Graphics Computer Systems, Inc.\n"
			"\n"
			"Permission to use, copy, modify, distribute and sell this software and its documentation for any purpose is hereby granted without fee, provided that the above copyright notice appear in all copies and that both that copyright notice and this permission notice appear in supporting documentation. Silicon Graphics makes no representations about the suitability of this software for any purpose. It is provided \"as is\" without express or implied warranty."
	);

	list.emplace_back(
			"ReactPhysics3D",

			"https://www.reactphysics3d.com/",

			"zlib License",

			"Copyright (c) 2010-2022 Daniel Chappuis http://www.reactphysics3d.com\n"
			"\n"
			"This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.\n"
			"\n"
			"Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:\n"
			"    1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.\n"
			"    2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.\n"
			"    3. This notice may not be removed or altered from any source distribution."
	);

	list.emplace_back(
			"Simple OpenGL Image Library 2",

			"https://github.com/SpartanJ/SOIL2",

			"MIT No Attribution",

			"Copyright 2020 Martín Lucas Golini\n"
			"\n"
			"Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the \"Software\"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so.\n"
			"\n"
			"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE."
	);

	list.emplace_back(
			"Sol 2",

			"https://sol2.readthedocs.io",

			"MIT License",

			"Copyright (c) 2013-2022 Rapptz, ThePhD, and contributors\n"
			"\n"
			"Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the \"Software\"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:\n"
			"\n"
			"The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.\n"
			"\n"
			"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE."
	);

	list.emplace_back(
			"SML (State Machine Language)",

			"https://boost-ext.github.io/sml/",

			"Boost Software License 1.0",

			"Boost Software License - Version 1.0 - August 17th, 2003\n"
			"\n"
			"Permission is hereby granted, free of charge, to any person or organization obtaining a copy of the software and accompanying documentation covered by this license (the \"Software\") to use, reproduce, display, distribute, execute, and transmit the Software, and to prepare derivative works of the Software, and to permit third-parties to whom the Software is furnished to do so, all subject to the following:\n"
			"\n"
			"The copyright notices in the Software and this entire statement, including the above license grant, this restriction and the following disclaimer, must be included in all copies of the Software, in whole or in part, and all derivative works of the Software, unless such copies or derivative works are solely in the form of machine-executable object code generated by a source language processor.\n"
			"\n"
			"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE."
	);

	list.emplace_back(
			"SQLite",

			"https://www.sqlite.org",

			"Public Domain",

			"All of the code and documentation in SQLite has been dedicated to the public domain by the authors. All code authors, and representatives of the companies they work for, have signed affidavits dedicating their contributions to the public domain and originals of those signed affidavits are stored in a firesafe at the main offices of Hwaci. All contributors are citizens of countries that allow creative works to be dedicated into the public domain. Anyone is free to copy, modify, publish, use, compile, sell, or distribute the original SQLite code, either in source code form or as a compiled binary, for any purpose, commercial or non-commercial, and by any means.\n"
			"\n"
			"The previous paragraph applies to the deliverable code and documentation in SQLite - those parts of the SQLite library that you actually bundle and ship with a larger application. Some scripts used as part of the build process (for example the \"configure\" scripts generated by autoconf) might fall under other open-source licenses. Nothing from these build scripts ever reaches the final deliverable SQLite library, however, and so the licenses associated with those scripts should not be a factor in assessing your rights to copy and use the SQLite library.\n"
			"\n"
			"All of the deliverable code in SQLite has been written from scratch. No code has been taken from other projects or from the open internet. Every line of code can be traced back to its original author, and all of those authors have public domain dedications on file. So the SQLite code base is clean and is uncontaminated with licensed code from other projects."
	);

	list.emplace_back(
			"UTF8-CPP",

			"https://github.com/nemtrif/utfcpp",

			"Boost Software License 1.0",

			"Boost Software License - Version 1.0 - August 17th, 2003\n"
			"\n"
			"Permission is hereby granted, free of charge, to any person or organization obtaining a copy of the software and accompanying documentation covered by this license (the \"Software\") to use, reproduce, display, distribute, execute, and transmit the Software, and to prepare derivative works of the Software, and to permit third-parties to whom the Software is furnished to do so, all subject to the following:\n"
			"\n"
			"The copyright notices in the Software and this entire statement, including the above license grant, this restriction and the following disclaimer, must be included in all copies of the Software, in whole or in part, and all derivative works of the Software, unless such copies or derivative works are solely in the form of machine-executable object code generated by a source language processor.\n"
			"\n"
			"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE."
	);

	list.emplace_back(
			"UTF8-CPP",

			"https://github.com/nemtrif/utfcpp",

			"Boost Software License 1.0",

			"Boost Software License - Version 1.0 - August 17th, 2003\n"
			"\n"
			"Permission is hereby granted, free of charge, to any person or organization obtaining a copy of the software and accompanying documentation covered by this license (the \"Software\") to use, reproduce, display, distribute, execute, and transmit the Software, and to prepare derivative works of the Software, and to permit third-parties to whom the Software is furnished to do so, all subject to the following:\n"
			"\n"
			"The copyright notices in the Software and this entire statement, including the above license grant, this restriction and the following disclaimer, must be included in all copies of the Software, in whole or in part, and all derivative works of the Software, unless such copies or derivative works are solely in the form of machine-executable object code generated by a source language processor.\n"
			"\n"
			"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE."
	);

	list.emplace_back(
			"jc_voronoi",

			"https://github.com/JCash/voronoi",

			"MIT License",

			"Copyright (c) 2015 Mathias Westerdahl\n"
			"\n"
			"Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the \"Software\"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:\n"
			"\n"
			"The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.\n"
			"\n"
			"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE."
	);

	list.emplace_back(
			"Vide / Cereal",

			"https://github.com/cpplibv/vide (fork)\n"
			"https://github.com/USCiLab/cereal",

			"BSD 3-Clause \"New\" or \"Revised\" License",

			"Copyright (c) 2013-2022, Randolph Voorhies, Shane Grant\n"
			"All rights reserved.\n"
			"\n"
			"Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:\n"
			"    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.\n"
			"    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.\n"
			"    * Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.\n"
			"\n"
			"THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
	);

	list.emplace_back(
			"Wombat",

			"https://github.com/BrianSharpe/Wombat",

			"Custom",

			"An efficient texture-free GLSL procedural noise library\n"
			"Source: https://github.com/BrianSharpe/Wombat\n"
			"Derived from: https://github.com/BrianSharpe/GPU-Noise-Lib\n"
			"\n"
			"I'm not one for copyrights.  Use the code however you wish.\n"
			"All I ask is that credit be given back to the blog or myself when appropriate.\n"
			"And also to let me know if you come up with any changes, improvements, thoughts or interesting uses for this stuff. :)\n"
			"Thanks!\n"
			"\n"
			"Brian Sharpe"
	);

	list.emplace_back(
			"Zstandard",

			"https://github.com/facebook/zstd",

			"BSD License",

			"Copyright (c) Meta Platforms, Inc. and affiliates. All rights reserved.\n"
			"\n"
			"Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:\n"
			"\n"
			" * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.\n"
			" * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.\n"
			" * Neither the name Facebook, nor Meta, nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.\n"
			"\n"
			"THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
	);

	return list;
}

// -------------------------------------------------------------------------------------------------

} // namespace star
