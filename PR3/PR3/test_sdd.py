import unittest
from budget_planner import BudgetPlanner

class TestBudgetPlannerSDD(unittest.TestCase):
    def test_example_1(self):
        bp = BudgetPlanner()
        bp.set_budget(1000)
        bp.add_expense("Food", 200)
        bp.add_expense("Transport", 100)
        self.assertEqual(bp.remaining_budget(), 700)

    def test_example_2_insufficient(self):
        bp = BudgetPlanner()
        bp.set_budget(500)
        with self.assertRaises(ValueError):
            bp.add_expense("Rent", 600)

    def test_example_3_report(self):
        bp = BudgetPlanner()
        bp.set_budget(800)
        bp.add_expense("Food", 300)
        bp.add_expense("Food", 100)
        bp.add_expense("Transport", 50)
        report = bp.report_by_category()
        self.assertEqual(report["Food"], 400)
        self.assertEqual(report["Transport"], 50)

if __name__ == "__main__":
    unittest.main()