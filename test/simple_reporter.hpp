// File: TestReporter.hpp Author: Vader Created on 2017. január 13., 20:29

#pragma once

#include <reporters/catch_reporter_compact.hpp>

namespace Catch {

struct VCompReporter : StreamingReporterBase {
	VCompReporter(ReporterConfig const& _config)
		: StreamingReporterBase(_config) { }
	virtual ~VCompReporter() { }
	static std::string getDescription() {
		return "Reports test results on a single line, suitable for IDEs";
	}
	virtual ReporterPreferences getPreferences() const override {
		ReporterPreferences prefs;
		prefs.shouldRedirectStdOut = false;
		return prefs;
	}
	virtual void noMatchingTestCases(std::string const& spec) override {
		stream << "No test cases matched '" << spec << "'" << std::endl;
	}
	virtual void assertionStarting(AssertionInfo const&) override { }
	virtual bool assertionEnded(AssertionStats const& _assertionStats) override {
		AssertionResult const& result = _assertionStats.assertionResult;

		bool printInfoMessages = true;

		// Drop out if result was successful and we're not printing those
		if (!m_config->includeSuccessfulResults() && result.isOk()) {
			if (result.getResultType() != ResultWas::Warning)
				return false;
			printInfoMessages = false;
		}

		AssertionPrinter printer(stream, _assertionStats, printInfoMessages);
		printer.print();

		stream << std::endl;
		return true;
	}
	virtual void testRunEnded(TestRunStats const& _testRunStats) override {
		printTotalsDivider(_testRunStats.totals);
		printTotals(_testRunStats.totals);
		stream << std::endl;
		StreamingReporterBase::testRunEnded(_testRunStats);
	}

private:
	struct SummaryColumn {
		SummaryColumn(std::string const& _label, Colour::Code _colour)
			: label(_label),
			colour(_colour) { }
		SummaryColumn addRow(std::size_t count) {
			std::ostringstream oss;
			oss << count;
			std::string row = oss.str();
			for (std::vector<std::string>::iterator it = rows.begin(); it != rows.end(); ++it) {
				while (it->size() < row.size())
					*it = " " + *it;
				while (it->size() > row.size())
					row = " " + row;
			}
			rows.push_back(row);
			return *this;
		}

		std::string label;
		Colour::Code colour;
		std::vector<std::string> rows;

	};

	class AssertionPrinter {
	public:
		AssertionPrinter(std::ostream& _stream, AssertionStats const& _stats, bool _printInfoMessages)
			: stream(_stream)
			, stats(_stats)
			, result(_stats.assertionResult)
			, messages(_stats.infoMessages)
			, itMessage(_stats.infoMessages.begin())
			, printInfoMessages(_printInfoMessages) { }
		void print() {
			itMessage = messages.begin();

			switch (result.getResultType()) {
			case ResultWas::Ok:
				printResultType(Colour::ResultSuccess, passedString());
				printSourceInfo();
				printOriginalExpression();
				printReconstructedExpression();
				if (!result.hasExpression())
					printRemainingMessages(Colour::None);
				else
					printRemainingMessages();
				break;
			case ResultWas::ExpressionFailed:
				if (result.isOk())
					printResultType(Colour::ResultSuccess, failedString() + std::string(" - but was ok"));
				else
					printResultType(Colour::Error, failedString());
				printSourceInfo();
				printOriginalExpression();
				printReconstructedExpression();
				printRemainingMessages();
				break;
			case ResultWas::ThrewException:
				printResultType(Colour::Error, failedString());
				printSourceInfo();
				printIssue("unexpected exception with message:");
				printMessage();
				printExpressionWas();
				printRemainingMessages();
				break;
			case ResultWas::FatalErrorCondition:
				printResultType(Colour::Error, failedString());
				printSourceInfo();
				printIssue("fatal error condition with message:");
				printMessage();
				printExpressionWas();
				printRemainingMessages();
				break;
			case ResultWas::DidntThrowException:
				printResultType(Colour::Error, failedString());
				printSourceInfo();
				printIssue("expected exception, got none");
				printExpressionWas();
				printRemainingMessages();
				break;
			case ResultWas::Info:
				printResultType(Colour::None, "info");
				printSourceInfo();
				printMessage();
				printRemainingMessages();
				break;
			case ResultWas::Warning:
				printResultType(Colour::None, "warning");
				printSourceInfo();
				printMessage();
				printRemainingMessages();
				break;
			case ResultWas::ExplicitFailure:
				printResultType(Colour::Error, failedString());
				printSourceInfo();
				printIssue("explicitly");
				printRemainingMessages(Colour::None);
				break;
				// These cases are here to prevent compiler warnings
			case ResultWas::Unknown:
			case ResultWas::FailureBit:
			case ResultWas::Exception:
				printResultType(Colour::Error, "** internal error **");
				printSourceInfo();
				break;
			}
		}

	private:
		static Colour::Code dimColour() {
			return Colour::FileName;
		}
		static const char* failedString() {
			return "FAILED";
		}
		static const char* passedString() {
			return "PASSED";
		}
		void printSourceInfo() const {
			Colour colourGuard(Colour::FileName);
			stream << result.getSourceInfo() << ":";
		}
		void printResultType(Colour::Code colour, std::string passOrFail) const {
			if (!passOrFail.empty()) {
				{
					Colour colourGuard(colour);
					stream << passOrFail;
				}
				stream << ": ";
			}
		}
		void printIssue(std::string issue) const {
			stream << " " << issue;
		}
		void printExpressionWas() {
			if (result.hasExpression()) {
				stream << ";";
				{
					Colour colour(dimColour());
					stream << " expression was:";
				}
				printOriginalExpression();
			}
		}
		void printOriginalExpression() const {
			if (result.hasExpression()) {
				stream << "\n\t" << result.getExpression();
			}
		}
		void printReconstructedExpression() const {
			if (result.hasExpandedExpression()) {
				{
					Colour colour(dimColour());
					stream << "\n\tfor: ";
				}
				stream << result.getExpandedExpression();
			}
		}
		void printMessage() {
			if (itMessage != messages.end()) {
				stream << " '" << itMessage->message << "'";
				++itMessage;
			}
		}
		void printRemainingMessages(Colour::Code colour = dimColour()) {
			if (itMessage == messages.end())
				return;

			// using messages.end() directly yields compilation error:
			std::vector<MessageInfo>::const_iterator itEnd = messages.end();
			const std::size_t N = static_cast<std::size_t> (std::distance(itMessage, itEnd));

			{
				Colour colourGuard(colour);
				stream << " with " << pluralise(N, "message") << ":";
			}

			for (; itMessage != itEnd;) {
				// If this assertion is a warning ignore any INFO messages
				if (printInfoMessages || itMessage->type != ResultWas::Info) {
					stream << " '" << itMessage->message << "'";
					if (++itMessage != itEnd) {
						Colour colourGuard(dimColour());
						stream << " and";
					}
				}
			}
		}

	private:
		std::ostream& stream;
		AssertionStats const& stats;
		AssertionResult const& result;
		std::vector<MessageInfo> messages;
		std::vector<MessageInfo>::const_iterator itMessage;
		bool printInfoMessages;
	};

	// Colour, message variants:
	// - white: No tests ran.
	// -   red: Failed [both/all] N test cases, failed [both/all] M assertions.
	// - white: Passed [both/all] N test cases (no assertions).
	// -   red: Failed N tests cases, failed M assertions.
	// - green: Passed [both/all] N tests cases with M assertions.
	std::string bothOrAll(std::size_t count) const {
		return count == 1 ? "" : count == 2 ? "both " : "all ";
	}
	//	void printTotals(const Totals& totals) const {
	//		if (totals.testCases.total() == 0) {
	//			stream << "No tests ran.";
	//		} else if (totals.testCases.failed == totals.testCases.total()) {
	//			Colour colour(Colour::ResultError);
	//			const std::string qualify_assertions_failed =
	//					totals.assertions.failed == totals.assertions.total() ?
	//					bothOrAll(totals.assertions.failed) : "";
	//			stream <<
	//					"Failed " << bothOrAll(totals.testCases.failed)
	//					<< pluralise(totals.testCases.failed, "test case") << ", "
	//					"failed " << qualify_assertions_failed <<
	//					pluralise(totals.assertions.failed, "assertion") << ".";
	//		} else if (totals.assertions.total() == 0) {
	//			stream <<
	//					"Passed " << bothOrAll(totals.testCases.total())
	//					<< pluralise(totals.testCases.total(), "test case")
	//					<< " (no assertions).";
	//		} else if (totals.assertions.failed) {
	//			Colour colour(Colour::ResultError);
	//			stream <<
	//					"Failed " << pluralise(totals.testCases.failed, "test case") << ", "
	//					"failed " << pluralise(totals.assertions.failed, "assertion") << ".";
	//		} else {
	//			Colour colour(Colour::ResultSuccess);
	//			stream <<
	//					"Passed " << bothOrAll(totals.testCases.passed)
	//					<< pluralise(totals.testCases.passed, "test case") <<
	//					" with " << pluralise(totals.assertions.passed, "assertion") << ".";
	//		}
	//	}
	void printTotals(Totals const& totals) {
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
	void printSummaryRow(std::string const& label, std::vector<SummaryColumn> const& cols, std::size_t row) {
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
	void printTotalsDivider(Totals const& totals) {
		if (totals.testCases.total() > 0) {
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
		} else {
			stream << Colour(Colour::Warning) << std::string(CATCH_CONFIG_CONSOLE_WIDTH - 1, '-');
		}
		stream << "\n";
	}
	void printSummaryDivider() {
		stream << getLineOfChars<'-'>() << "\n";
	}
};

INTERNAL_CATCH_REGISTER_REPORTER("vcomp", VCompReporter)

}
