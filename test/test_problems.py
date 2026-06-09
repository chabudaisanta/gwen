from __future__ import annotations

from pathlib import Path

from helpers.discover import Problem
from helpers.runner import compile_solution, run_case

_exe_cache: dict[str, Path] = {}


def _exe_for(problem: Problem) -> Path:
    cached = _exe_cache.get(problem.problem_id)
    if cached is not None:
        return cached
    exe = compile_solution(problem)
    _exe_cache[problem.problem_id] = exe
    return exe


def test_problem_case(problem: Problem, case) -> None:
    run_case(_exe_for(problem), case, timeout_sec=problem.timeout_sec)
