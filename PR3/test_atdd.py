import unittest
from budget_planner import BudgetPlanner

class TestBudgetPlannerATDD(unittest.TestCase):
    def setUp(self):
        self.bp = BudgetPlanner()

    def test_sufficient_budget_expense_with_check(self):
        """Сценарий: добавление расхода в рамках бюджета с проверкой can_add_expense"""
        self.bp.set_budget(1000)

        can_add, message = self.bp.can_add_expense("Transport", 200)
        self.assertTrue(can_add, msg=message)  # Проверяем, что расход можно добавить

        self.bp.add_expense("Transport", 200)
        self.assertEqual(self.bp.remaining_budget(), 800)

    def test_insufficient_budget_expense_with_check(self):
        """Сценарий: попытка добавить расход больше бюджета с проверкой can_add_expense"""
        self.bp.set_budget(100)

        can_add, message = self.bp.can_add_expense("Rent", 200)
        self.assertFalse(can_add)
        self.assertEqual(message, "Недостаточно средств")

        # Проверяем, что при попытке добавить расход больше бюджета возникает ошибка
        with self.assertRaises(ValueError) as e:
            self.bp.add_expense("Rent", 200)
        self.assertEqual(str(e.exception), "Недостаточно средств")

if __name__ == "__main__":
    unittest.main()
