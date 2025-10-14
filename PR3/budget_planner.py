class BudgetPlanner:

    def __init__(self):
        self.total_budget = 0.0
        self.expenses = []

    def set_budget(self, amount):
        if amount < 0:
            raise ValueError("Бюджет не может быть отрицательным")
        self.total_budget = float(amount)

    def add_expense(self, category, amount):
        if amount < 0:
            raise ValueError("Расход не может быть отрицательным")
        if amount > self.remaining_budget():
            raise ValueError("Недостаточно средств")
        self.expenses.append({"category": category, "amount": float(amount)})

    def remaining_budget(self):
        spent = sum(e["amount"] for e in self.expenses)
        return self.total_budget - spent

    def can_add_expense(self, category, amount):
        if amount < 0:
            return False, "Расход не может быть отрицательным"
        if amount > self.remaining_budget():
            return False, "Недостаточно средств"
        return True, "Можно добавить расход"


    def report_by_category(self):
        result = {}
        for e in self.expenses:
            result.setdefault(e["category"], 0.0)
            result[e["category"]] += e["amount"]
        return result

    def add_expense_sdd(self, category, amount):
        if amount < 0:
            raise ValueError("Расход не может быть отрицательным")
        if amount > self.remaining_budget():
            raise ValueError("Недостаточно средств")
        self.expenses.append({"category": category, "amount": float(amount)})

    def remaining_budget_sdd(self):
        return self.remaining_budget()

