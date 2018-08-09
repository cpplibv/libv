// File: TestReporter.hpp Author: Vader Created on 2017. január 13., 20:29

#pragma once

// ext
#include <fmt/format.h>
// libv
#include <libv/utility/slice.hpp>
// std
#include <string_view>
#include <unordered_set>
// pro
#include <catch/catch.hpp>


// NOTE: This file is a mess. No time, don't care.

namespace {

#ifndef LIBV_SHORT_PATH_CUTOFF
#    define LIBV_SHORT_PATH_CUTOFF 0
#endif

template <typename OS, typename Pos>
OS& streamSourcePosition(OS& os, const Pos& pos) {
	std::string_view file = pos.file;
	file.remove_prefix(LIBV_SHORT_PATH_CUTOFF);

	file = libv::slice_prefix_view(file, "tests/");
	file = libv::slice_prefix_view(file, "test/");

	return os << file << ":" << pos.line;
}

} // namespace

namespace Catch {

// Format:
//
//  FAILED: {file}:{line}: {yellow}{test_name}{/yellow} {tags}
//  	{source}: ok: {cyan}{expression}{/cyan}
//  		eval: {evaluation}
//
//  FAILED: {file}:{line}: {yellow}{test_name}{/yellow} {tags}
//  	{source}: {cyan}{expression}{/cyan}
//  		eval: {evaluation}
//
//  FAILED: {file}:{line}: {yellow}{test_name}{/yellow} {tags}
//  	{source}: unexpected exception in: {cyan}{expression}{/cyan}
//  		what: {exception}
//
//  FAILED: {file}:{line}: {yellow}{test_name}{/yellow} {tags}
//  	{source}: missing exception in: {cyan}{expression}{/cyan}
//
class LIBVMinimalReporter : public StreamingReporterBase<LIBVMinimalReporter> {
private:
	std::unordered_set<std::string> reported_tests;
	std::vector<SectionInfo> sections;

public:
	LIBVMinimalReporter(const ReporterConfig& config) : StreamingReporterBase(config) { }
	virtual ~LIBVMinimalReporter() = default;

	static std::string getDescription() {
		return "Reports test results in a compact minimalistic manner, suitable for developer";
	}

private:
	virtual ReporterPreferences getPreferences() const override {
		ReporterPreferences prefs;
		prefs.shouldRedirectStdOut = false;
		return prefs;
	}

	virtual void noMatchingTestCases(const std::string& spec) override {
		stream << "No test cases matched '" << spec << "'" << std::endl;
	}

	virtual void skipTest(const TestCaseInfo& testInfo) override {
		stream << "\nSKIPPED ";
		streamSourcePosition(stream, testInfo.lineInfo) << ": ";
		stream << "\u001B[33m" << testInfo.name << "\u001B[0m ";
		stream << testInfo.tagsAsString() << "\n";
	}

	virtual void assertionStarting(const AssertionInfo&) override { }

	virtual bool assertionEnded(const AssertionStats& _assertionStats) override {
		const AssertionResult& result = _assertionStats.assertionResult;
		bool printInfoMessages = true;

		// Drop out if result was successful and we're not printing those
		if (!m_config->includeSuccessfulResults() && result.isOk()) {
			if (result.getResultType() != ResultWas::Warning)
				return false;
			printInfoMessages = false;
		}

		if (!_assertionStats.assertionResult.succeeded() && reported_tests.emplace(currentTestCaseInfo->name).second) {
			stream << "\nFAILED: ";
			streamSourcePosition(stream, currentTestCaseInfo->lineInfo) << ": ";
			stream << "\u001B[33m" << currentTestCaseInfo->name << "\u001B[0m ";
			stream << currentTestCaseInfo->tagsAsString() << "\n";
		}

		AssertionPrinter printer(stream, _assertionStats, printInfoMessages, currentTestCaseInfo);
//		AssertionPrinter printer(stream, _assertionStats, printInfoMessages, currentTestCaseInfo, sections, sections.size() - 1);
		printer.print();

		stream << std::endl;
		return true;
	}

	virtual void testRunEnded(const TestRunStats& _testRunStats) override {
		printTotalsDivider(_testRunStats.totals);
		printTotals(_testRunStats.totals);
		stream << std::endl;
		StreamingReporterBase::testRunEnded(_testRunStats);
	}

//	virtual void sectionStarting(const SectionInfo& sectionInfo) override {
//		m_sectionStack.emplace_back(sectionInfo);
//		sections = m_sectionStack;
//	}
//
//	virtual void sectionEnded(const SectionStats&) override {
//		m_sectionStack.pop_back();
//	}

private:
	struct SummaryColumn {
		std::string label;
		Colour::Code colour;
		std::vector<std::string> rows;

		SummaryColumn(const std::string& label, Colour::Code colour) : label(label), colour(colour) { }

		SummaryColumn addRow(std::size_t count) {
			std::ostringstream oss;
			oss << count;
			std::string row = oss.str();
			for (auto it : rows) {
				while (it.size() < row.size())
					it = " " + it;
				while (it.size() > row.size())
					row = " " + row;
			}
			rows.push_back(row);
			return *this;
		}
	};

	class AssertionPrinter {
	private:
		std::ostream& stream;
		const AssertionStats& stats;
		const AssertionResult& result;
		LazyStat<TestCaseInfo>& testInfo;
//		std::vector<SectionInfo>& sections;
//		int sectionsDirty;
		std::vector<MessageInfo> messages;
		std::vector<MessageInfo>::const_iterator itMessage;
		bool printInfoMessages;

	public:
		AssertionPrinter(std::ostream& _stream, const AssertionStats& _stats, bool _printInfoMessages,
//				LazyStat<TestCaseInfo>& testInfo, std::vector<SectionInfo>& sections, int sectionsDirty) :
				LazyStat<TestCaseInfo>& testInfo) :
			stream(_stream),
			stats(_stats),
			result(_stats.assertionResult),
			testInfo(testInfo),
//			sections(sections),
//			sectionsDirty(sectionsDirty),
			messages(_stats.infoMessages),
			itMessage(_stats.infoMessages.begin()),
			printInfoMessages(_printInfoMessages) { }

		void print() {
			itMessage = messages.begin();

			std::string format;

			switch (result.getResultType()) {
			case ResultWas::Ok:
				break;
			case ResultWas::ExpressionFailed:
				if (result.isOk())
					format = ""
//						"{section}"
						"{source}: ok: \u001B[36m{expression}\u001B[0m\n"
						"\t\teval: {evaluation}";
				else
					format = ""
//						"{section}"
						"{source}: \u001B[36m{expression}\u001B[0m\n"
						"\t\teval: {evaluation}";
				break;
			case ResultWas::ThrewException:
				format = ""
//						"{section}"
						"{source}: unexpected exception in: \u001B[36m{expression}\u001B[0m\n"
						"\t\twhat: {exception}";
				break;
			case ResultWas::FatalErrorCondition:
				printResultType(Colour::Error, "");
				printIssue("\tfatal error condition with message:");
				printMessage();
				printOriginalExpression();
				printSourceInfo();
				break;
			case ResultWas::DidntThrowException:
				format = ""
//						"{section}"
						"{source}: missing exception in: \u001B[36m{expression}\u001B[0m";
				break;
			case ResultWas::Info:
				printResultType(Colour::None, "info");
				printMessage();
				printSourceInfo();
				break;
			case ResultWas::Warning:
				printResultType(Colour::None, "warning");
				printMessage();
				printSourceInfo();
				break;
			case ResultWas::ExplicitFailure:
				printResultType(Colour::Error, "failure");
				printIssue("explicitly");
				printSourceInfo();
				break;
				// These cases are here to prevent compiler warnings
			case ResultWas::Unknown:
			case ResultWas::FailureBit:
			case ResultWas::Exception:
				printResultType(Colour::Error, "** internal error **");
				printSourceInfo();
				break;
			}

			std::string source = result.getSourceInfo().file != testInfo->lineInfo.file ?
					fmt::format("\t{}:{}", result.getSourceInfo().file + LIBV_SHORT_PATH_CUTOFF + 5, result.getSourceInfo().line) :
					fmt::format("{:6}", result.getSourceInfo().line);

//			std::string section;
//			if (sectionsDirty > 0 && !sections.empty()) {
//				for (size_t i = sections.size() - sectionsDirty; i < sections.size(); ++i) {
//
//					std::string section_source = sections[i].lineInfo.file != testInfo->lineInfo.file ?
//							fmt::format("\t{}:{}", sections[i].lineInfo.file + LIBV_SHORT_PATH_CUTOFF + 5, sections[i].lineInfo.line) :
//							fmt::format("{:6}", sections[i].lineInfo.line);
//
//					section += fmt::format("{}: Section: {}\n", section_source, sections[i].name);
//				}
//			}

			stream << fmt::format(format,
//					fmt::arg("section", section),
					fmt::arg("source", source),
					fmt::arg("expression", result.hasExpression() ? result.getExpression() : ""),
					fmt::arg("evaluation", result.hasExpandedExpression() ? result.getExpandedExpression() : ""),
					fmt::arg("exception", result.hasMessage() ? getMessage() : std::string(""))
					);

			printRemainingMessages();
		}

	private:
		void printSourceInfo() const {
//			Colour colourGuard(Colour::FileName);
			stream << " @ ";
			streamSourcePosition(stream, result.getSourceInfo()) << ":";
		}

		void printResultType(Colour::Code colour, std::string passOrFail) const {
			if (!passOrFail.empty()) {
				{
					Colour colourGuard(colour);
					stream << passOrFail;
				}
				stream << " ";
			}
		}

		void printIssue(std::string issue) const {
			stream << " " << issue;
		}

		void printOriginalExpression() const {
			if (result.hasExpression())
				stream << " " << result.getExpression();
		}

		void printMessage() {
			if (itMessage != messages.end()) {
				stream << " '" << itMessage->message << "'";
				++itMessage;
			}
		}

		std::string getMessage() {
			if (itMessage != messages.end())
				return (itMessage++)->message;
			else
				return std::string("");
		}

		void printRemainingMessages() {
			auto end = messages.cend();

			if (itMessage == end)
				return;

			stream << " with " << pluralise(std::distance(itMessage, end), "message") << ":";

			while(itMessage != end) {
				// If this assertion is a warning ignore any INFO messages
				if (printInfoMessages || itMessage->type != ResultWas::Info) {
					stream << " '" << itMessage->message << "'";
					if (++itMessage != end) {
						stream << " and";
					}
				}
			}
		}
	};

	// Colour, message variants:
	// - white: No tests ran.
	// -   red: Failed [both/all] N test cases, failed [both/all] M assertions.
	// - white: Passed [both/all] N test cases (no assertions).
	// -   red: Failed N tests cases, failed M assertions.
	// - green: Passed [both/all] N tests cases with M assertions.

	void printTotals(const Totals& totals) {
		if (totals.testCases.total() == 0) {
			stream << Colour(Colour::Warning) << "No tests ran\n";
		} else if (totals.assertions.total() > 0 && totals.testCases.allPassed()) {
			stream << Colour(Colour::ResultSuccess) << "All tests passed";
			stream << " ("
					<< pluralise(totals.assertions.passed, "assertion") << " in "
					<< pluralise(totals.testCases.passed, "test case") << ")"
					<< "\n";
		} else {

			std::vector<SummaryColumn> columns;
			columns.push_back(SummaryColumn("", Colour::None)
					.addRow(totals.testCases.total())
					.addRow(totals.assertions.total()));
			columns.push_back(SummaryColumn("passed", Colour::Success)
					.addRow(totals.testCases.passed)
					.addRow(totals.assertions.passed));
			columns.push_back(SummaryColumn("failed", Colour::ResultError)
					.addRow(totals.testCases.failed)
					.addRow(totals.assertions.failed));
			columns.push_back(SummaryColumn("failed as expected", Colour::ResultExpectedFailure)
					.addRow(totals.testCases.failedButOk)
					.addRow(totals.assertions.failedButOk));

			printSummaryRow("test cases", columns, 0);
			printSummaryRow("assertions", columns, 1);
		}
	}

	void printSummaryRow(const std::string& label, const std::vector<SummaryColumn>& cols, std::size_t row) {
		for (std::vector<SummaryColumn>::const_iterator it = cols.begin(); it != cols.end(); ++it) {
			std::string value = it->rows[row];
			if (it->label.empty()) {
				stream << label << ": ";
				if (value != "0")
					stream << value;
				else
					stream << Colour(Colour::Warning) << "- none -";
			} else if (value != "0") {
				stream << Colour(Colour::LightGrey) << " | ";
				stream << Colour(it->colour)
						<< value << " " << it->label;
			}
		}
		stream << "\n";
	}

	static std::size_t makeRatio(std::size_t number, std::size_t total) {
		std::size_t ratio = total > 0 ? CATCH_CONFIG_CONSOLE_WIDTH * number / total : 0;
		return ( ratio == 0 && number > 0) ? 1 : ratio;
	}

	static std::size_t& findMax(std::size_t& i, std::size_t& j, std::size_t& k) {
		if (i > j && i > k)
			return i;
		else if (j > k)
			return j;
		else
			return k;
	}

	void printTotalsDivider(const Totals& totals) {
		if (totals.testCases.total() == 0) {
			stream << Colour(Colour::Warning) << std::string(CATCH_CONFIG_CONSOLE_WIDTH - 1, '-');
		} else {
			std::size_t failedRatio = makeRatio(totals.testCases.failed, totals.testCases.total());
			std::size_t failedButOkRatio = makeRatio(totals.testCases.failedButOk, totals.testCases.total());
			std::size_t passedRatio = makeRatio(totals.testCases.passed, totals.testCases.total());
			while (failedRatio + failedButOkRatio + passedRatio < CATCH_CONFIG_CONSOLE_WIDTH - 1)
				findMax(failedRatio, failedButOkRatio, passedRatio)++;
			while (failedRatio + failedButOkRatio + passedRatio > CATCH_CONFIG_CONSOLE_WIDTH - 1)
				findMax(failedRatio, failedButOkRatio, passedRatio)--;

			stream << Colour(Colour::Error) << std::string(failedRatio, '-');
			stream << Colour(Colour::ResultExpectedFailure) << std::string(failedButOkRatio, '-');
			if (totals.testCases.allPassed())
				stream << Colour(Colour::ResultSuccess) << std::string(passedRatio, '-');
			else
				stream << Colour(Colour::Success) << std::string(passedRatio, '-');
		}
		stream << '\n';
	}
};

CATCH_REGISTER_REPORTER("libv_minimal", LIBVMinimalReporter)

} // namespace Catch
