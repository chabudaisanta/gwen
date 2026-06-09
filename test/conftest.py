from __future__ import annotations

import sys
from pathlib import Path

import pytest

TEST_DIR = Path(__file__).resolve().parent
if str(TEST_DIR) not in sys.path:
    sys.path.insert(0, str(TEST_DIR))

from helpers.discover import Problem, discover_problems  # noqa: E402


def pytest_addoption(parser: pytest.Parser) -> None:
    parser.addoption(
        "--problem",
        action="append",
        default=[],
        help="run only problems whose id contains this substring (repeatable)",
    )


def _filter_problems(problems: list[Problem], patterns: list[str]) -> list[Problem]:
    if not patterns:
        return problems
    filtered: list[Problem] = []
    for problem in problems:
        if any(p in problem.problem_id for p in patterns):
            filtered.append(problem)
    return filtered


def pytest_generate_tests(metafunc: pytest.Metafunc) -> None:
    if "problem" not in metafunc.fixturenames or "case" not in metafunc.fixturenames:
        return

    patterns = metafunc.config.getoption("--problem")
    problems = _filter_problems(discover_problems(), patterns)

    params: list[pytest.ParameterSet] = []
    for problem in problems:
        for case in problem.cases:
            params.append(
                pytest.param(
                    problem,
                    case,
                    id=f"{problem.problem_id}/{case.name}",
                )
            )

    if not params and patterns:
        metafunc.parametrize("problem,case", [])
        return

    metafunc.parametrize("problem,case", params)
