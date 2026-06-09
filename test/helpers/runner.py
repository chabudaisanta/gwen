from __future__ import annotations

import hashlib
import os
import subprocess
from pathlib import Path

from .discover import Case, Problem

ROOT = Path(__file__).resolve().parents[2]
SRC_DIR = ROOT / "src"
BUILD_DIR = ROOT / "debug" / "test_build"

DEFAULT_CXXFLAGS = ["-std=c++20", f"-I{SRC_DIR}", "-O2", "-DLOCAL"]


def normalize_output(text: str) -> str:
    lines = [line.rstrip() for line in text.splitlines()]
    while lines and lines[-1] == "":
        lines.pop()
    return "\n".join(lines)


def compile_solution(problem: Problem, *, cxx: str | None = None) -> Path:
    cxx = cxx or os.environ.get("CXX", "g++")
    BUILD_DIR.mkdir(parents=True, exist_ok=True)

    source_bytes = problem.solution.read_bytes()
    digest = hashlib.sha256(source_bytes).hexdigest()[:12]
    exe = BUILD_DIR / f"{problem.problem_id}_{digest}.exe"

    if exe.is_file() and exe.stat().st_mtime >= problem.solution.stat().st_mtime:
        return exe

    cmd = [cxx, *DEFAULT_CXXFLAGS, str(problem.solution), "-o", str(exe)]
    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0:
        msg = (
            f"compile failed: {problem.solution}\n"
            f"command: {' '.join(cmd)}\n"
            f"{result.stderr}"
        )
        raise RuntimeError(msg)

    return exe


def run_case(
    exe: Path,
    case: Case,
    *,
    timeout_sec: float,
) -> None:
    input_text = case.input_path.read_text(encoding="utf-8")
    expected = normalize_output(case.output_path.read_text(encoding="utf-8"))

    try:
        result = subprocess.run(
            [str(exe)],
            input=input_text,
            capture_output=True,
            text=True,
            timeout=timeout_sec,
        )
    except subprocess.TimeoutExpired as e:
        raise AssertionError(
            f"TLE ({timeout_sec}s): {case.name}\n"
            f"partial stdout:\n{e.stdout or ''}"
        ) from e

    if result.returncode != 0:
        raise AssertionError(
            f"runtime error (exit {result.returncode}): {case.name}\n"
            f"stderr:\n{result.stderr}"
        )

    actual = normalize_output(result.stdout)
    if actual != expected:
        raise AssertionError(
            f"WA: {case.name}\n"
            f"--- expected ---\n{expected}\n"
            f"--- actual ---\n{actual}"
        )


def run_problem(problem: Problem, *, cxx: str | None = None) -> None:
    if not problem.cases:
        raise AssertionError(f"no cases: {problem.problem_id}")

    exe = compile_solution(problem, cxx=cxx)
    for case in problem.cases:
        run_case(exe, case, timeout_sec=problem.timeout_sec)
