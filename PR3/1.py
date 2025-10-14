# budget_planner.py

class BudgetPlanner:
    """
    Класс BudgetPlanner реализует управление бюджетом.
    Реализованы функции для прохождения тестов TDD, ATDD, BDD и SDD.
    """

    def __init__(self):
        self.total_budget = 0.0
        self.expenses = []  # Список словарей: {"category": str, "amount": float}

    # -----------------------------
    # Функции для TDD (Unit-тесты)
    # -----------------------------
    def set_budget(self, amount):
        """
        Устанавливает общий бюджет.
        Тесты TDD проверяют:
        - установка бюджета
        - ошибка при отрицательном бюджете
        """
        if amount < 0:
            raise ValueError("Бюджет не может быть отрицательным")
        self.total_budget = float(amount)

    def add_expense(self, category, amount):
        """
        Добавляет расход в категорию.
        Тесты TDD проверяют:
        - добавление расхода
        - ошибка при отрицательном расходе
        - ошибка при превышении бюджета
        """
        if amount < 0:
            raise ValueError("Расход не может быть отрицательным")
        if amount > self.remaining_budget():
            raise ValueError("Недостаточно средств")
        self.expenses.append({"category": category, "amount": float(amount)})

    def remaining_budget(self):
        """
        Возвращает оставшийся бюджет.
        Тесты TDD проверяют корректность расчета остатка.
        """
        spent = sum(e["amount"] for e in self.expenses)
        return self.total_budget - spent

    # -----------------------------
    # Функции для ATDD (Acceptance Tests)
    # -----------------------------
    def can_add_expense(self, category, amount):
        """
        Проверка возможности добавить расход без превышения бюджета.
        Приемочные тесты ATDD используют эту функцию для проверки
        бизнес-логики перед добавлением расхода.
        """
        if amount < 0:
            return False, "Расход не может быть отрицательным"
        if amount > self.remaining_budget():
            return False, "Недостаточно средств"
        return True, "Можно добавить расход"

    # -----------------------------
    # Функции для BDD (Behavior Driven Development)
    # -----------------------------
    def report_by_category(self):
        """
        Возвращает отчет по категориям расходов.
        Сценарии BDD используют этот отчет для проверки
        ожидаемого поведения системы по категориям.
        """
        result = {}
        for e in self.expenses:
            result.setdefault(e["category"], 0.0)
            result[e["category"]] += e["amount"]
        return result

    # -----------------------------
    # Функции для SDD (Specification by Example)
    # -----------------------------
    def add_expense_sdd(self, category, amount):
        """
        Функция для тестов по примерам (SDD).
        Реализует добавление расхода и проверку бизнес-правил
        в соответствии с таблицами спецификаций.
        """
        # В SDD может использоваться специфическая обработка ошибок
        if amount < 0:
            raise ValueError("Расход не может быть отрицательным")
        if amount > self.remaining_budget():
            raise ValueError("Недостаточно средств")
        self.expenses.append({"category": category, "amount": float(amount)})

    def remaining_budget_sdd(self):
        """
        Отчет для SDD по оставшемуся бюджету.
        Позволяет напрямую сверять значения с таблицами примеров.
        """
        return self.remaining_budget()

