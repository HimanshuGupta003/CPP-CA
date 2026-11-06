"""Optional black-box tests. Only Python's standard library is used."""
import subprocess
import sys
import unittest
from pathlib import Path

EXE = Path(sys.argv[1] if len(sys.argv) > 1 else "build/newspaper_ca1.exe").resolve()
sys.argv = sys.argv[:1]


def record(key=101, title="Community Clean-up", reporter="Neha Sharma", category=1, words=350):
    return f"1\n{key}\n{title}\n{reporter}\n{category}\n{words}\n"


class EditorialDeskTests(unittest.TestCase):
    def run_app(self, user_input):
        result = subprocess.run([str(EXE)], input=user_input, text=True,
                                capture_output=True, timeout=10)
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(result.stderr, "")
        return result.stdout

    def test_01_empty_list(self):
        self.assertIn("No articles available.", self.run_app("2\n3\n"))

    def test_02_create_and_read(self):
        output = self.run_app(record() + "2\n3\n")
        for value in ("Total articles: 1", "Article ID : 101", "Community Clean-up",
                      "Neha Sharma", "Local News", "Word count : 350"):
            self.assertIn(value, output)

    def test_03_duplicate_id_retry(self):
        output = self.run_app(record() + "1\n101\n102\nSecond Story\nAman\n2\n600\n2\n3\n")
        self.assertIn("This ID already exists.", output)
        self.assertEqual(output.count("Article added successfully."), 2)
        self.assertIn("Total articles: 2", output)
        self.assertEqual(output.count("Article ID : 101"), 1)
        self.assertIn("Article ID : 102", output)

    def test_04_invalid_menu(self):
        output = self.run_app("\nabc\n0\n4\n1.5\n2x\n9999999999999999999999\n3\n")
        self.assertEqual(output.count("Enter a whole number from 1 to 3."), 7)

    def test_05_invalid_ids(self):
        output = self.run_app("1\n0\n-1\n10000\nabc\n12x\n2.5\n999999999999999999999\n"
                              "7\nValid Story\nReporter\n1\n1\n2\n3\n")
        self.assertEqual(output.count("Enter a whole number from 1 to 9999."), 7)
        self.assertIn("Article ID : 7", output)

    def test_06_blank_and_long_title(self):
        output = self.run_app("1\n1\n\n   \n" + "T" * 81 + "\nValid Title\nReporter\n1\n1\n2\n3\n")
        self.assertEqual(output.count("Enter 1 to 80 characters"), 3)
        self.assertIn("Title      : Valid Title", output)

    def test_07_blank_and_long_reporter(self):
        output = self.run_app("1\n1\nStory\n\n\t\n" + "R" * 41 + "\nValid Name\n1\n1\n2\n3\n")
        self.assertEqual(output.count("Enter 1 to 40 characters"), 3)
        self.assertIn("Reporter   : Valid Name", output)

    def test_08_invalid_category(self):
        output = self.run_app("1\n1\nStory\nReporter\n0\n4\nabc\n2.5\n3\n5\n2\n3\n")
        self.assertEqual(output.count("Enter a whole number from 1 to 3."), 4)
        self.assertIn("Category   : Events", output)

    def test_09_invalid_word_count(self):
        output = self.run_app("1\n1\nStory\nReporter\n1\n0\n-2\n5001\nabc\n1.5\n2x\n"
                              "100\n2\n3\n")
        self.assertEqual(output.count("Enter a whole number from 1 to 5000."), 6)
        self.assertIn("Word count : 100", output)

    def test_10_numeric_boundaries(self):
        output = self.run_app(record(key=1, words=1) + record(key=9999, words=5000) + "2\n3\n")
        for value in ("Article ID : 1\n", "Article ID : 9999\n", "Word count : 1\n",
                      "Word count : 5000\n", "Total articles: 2"):
            self.assertIn(value, output)

    def test_11_text_length_boundaries(self):
        output = self.run_app(record(key=1, title="T", reporter="R") +
                              record(key=2, title="T" * 80, reporter="R" * 40) + "2\n3\n")
        self.assertIn("Title      : " + "T" * 80 + "\n", output)
        self.assertIn("Reporter   : " + "R" * 40 + "\n", output)
        self.assertIn("Total articles: 2", output)

    def test_12_outer_spaces_are_trimmed(self):
        output = self.run_app(record(title="  Street  Fair  ", reporter="\tNeha Sharma\t") + "2\n3\n")
        self.assertIn("Title      : Street  Fair\n", output)
        self.assertIn("Reporter   : Neha Sharma\n", output)

    def test_13_all_categories(self):
        output = self.run_app("".join(record(key=i, category=i) for i in range(1, 4)) + "2\n3\n")
        for category in ("Local News", "Education", "Events"):
            self.assertIn("Category   : " + category, output)

    def test_14_capacity_limit(self):
        output = self.run_app("".join(record(key=i) for i in range(1, 51)) + "1\n2\n3\n")
        self.assertEqual(output.count("Article added successfully."), 50)
        self.assertIn("Storage is full. Maximum 50 articles.", output)
        self.assertIn("Total articles: 50", output)
        self.assertEqual(output.count("Article ID : "), 50)

    def test_15_view_does_not_change_count(self):
        output = self.run_app(record() + "2\n2\n3\n")
        self.assertEqual(output.count("Total articles: 1"), 2)
        self.assertEqual(output.count("Article added successfully."), 1)

    def test_16_restart_clears_records(self):
        self.run_app(record() + "3\n")
        self.assertIn("No articles available.", self.run_app("2\n3\n"))

    def test_17_end_of_input_at_menu(self):
        output = self.run_app("")
        self.assertIn("Program closed.", output)

    def test_18_end_of_input_during_create(self):
        for data in ("1\n", "1\n1\n", "1\n1\nTitle\n", "1\n1\nTitle\nReporter\n",
                     "1\n1\nTitle\nReporter\n1\n"):
            with self.subTest(data=data):
                output = self.run_app(data)
                self.assertIn("Incomplete article was not saved.", output)
                self.assertNotIn("Article added successfully.", output)

    def test_19_sample_file(self):
        path = Path(__file__).resolve().parents[1] / "data" / (
            "52_CAP455_CA1_Community_Newspaper_Editorial_Desk_Sample_Input.txt")
        output = self.run_app(path.read_text(encoding="utf-8"))
        self.assertIn("No articles available.", output)
        self.assertIn("This ID already exists.", output)
        self.assertIn("Total articles: 2", output)


if __name__ == "__main__":
    unittest.main(verbosity=2)
