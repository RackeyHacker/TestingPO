import unittest
from budget_planner import BudgetPlanner

class TestBudgetPlannerTDD(unittest.TestCase):
    def setUp(self):
        self.bp = BudgetPlanner()

    def test_set_budget(self):
        for budget in [0, 100, 1000]:
            with self.subTest(budget=budget):
                self.bp.set_budget(budget)
                self.assertEqual(self.bp.total_budget, budget)

    def test_set_budget_negative(self):
        with self.assertRaises(ValueError):
            self.bp.set_budget(-50)

    def test_add_expense_within_budget(self):
        self.bp.set_budget(500)
        self.bp.add_expense("Food", 200)
        self.assertEqual(self.bp.remaining_budget(), 300)

    def test_add_expense_zero(self):
        self.bp.set_budget(100)
        self.bp.add_expense("Misc", 0)
        self.assertEqual(self.bp.remaining_budget(), 100)

    def test_add_expense_exceed_budget(self):
        self.bp.set_budget(100)
        with self.assertRaises(ValueError):
            self.bp.add_expense("Food", 200)

    def test_multiple_expenses_same_category(self):
        self.bp.set_budget(500)
        self.bp.add_expense("Food", 100)
        self.bp.add_expense("Food", 50)
        self.assertEqual(self.bp.remaining_budget(), 350)
        report = self.bp.report_by_category()
        self.assertEqual(report["Food"], 150)

if __name__ == "__main__":
    unittest.main()
