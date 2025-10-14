from behave import given, when, then
from budget_planner import BudgetPlanner
from behave import use_step_matcher

use_step_matcher("parse")

@given('я установил бюджет {amount:d}')
def step_given_set_budget(context, amount):
    context.bp = BudgetPlanner()
    context.bp.set_budget(amount)

@when('я добавляю расход "{category}" {amount:d}')
def step_when_add_expense(context, category, amount):
    try:
        context.bp.add_expense(category, amount)
        context.exception = None
    except ValueError as e:
        context.exception = e

@then('остаток бюджета должен быть {expected:d}')
def step_then_remaining_budget(context, expected):
    assert context.bp.remaining_budget() == expected

@then('я получаю сообщение "{message}"')
def step_then_exception_message(context, message):
    assert context.exception is not None
    assert str(context.exception) == message