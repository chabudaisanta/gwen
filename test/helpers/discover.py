from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path

import yaml

PROBLEMS_DIR = Path(__file__).resolve().parents[1] / "problems"


@dataclass(frozen=True)
class Case:
    name: str
    input_path: Path
    output_path: Path


@dataclass(frozen=True)
class Problem:
    problem_id: str
    root: Path
    solution: Path
    cases: tuple[Case, ...]
    title: str
    tags: tuple[str, ...]
    timeout_sec: float


def _load_meta(problem_dir: Path) -> dict:
    meta_path = problem_dir / "meta.yaml"
    if not meta_path.is_file():
        return {}
    with meta_path.open(encoding="utf-8") as f:
        data = yaml.safe_load(f)
    return data if isinstance(data, dict) else {}


def discover_problems(problems_dir: Path | None = None) -> list[Problem]:
    root = problems_dir or PROBLEMS_DIR
    if not root.is_dir():
        return []

    problems: list[Problem] = []
    for problem_dir in sorted(root.iterdir()):
        if not problem_dir.is_dir():
            continue

        solution = problem_dir / "solution.cpp"
        if not solution.is_file():
            continue

        cases_dir = problem_dir / "cases"
        cases: list[Case] = []
        if cases_dir.is_dir():
            for input_path in sorted(cases_dir.glob("*.in")):
                output_path = input_path.with_suffix(".out")
                if output_path.is_file():
                    cases.append(
                        Case(
                            name=input_path.stem,
                            input_path=input_path,
                            output_path=output_path,
                        )
                    )

        meta = _load_meta(problem_dir)
        problems.append(
            Problem(
                problem_id=problem_dir.name,
                root=problem_dir,
                solution=solution,
                cases=tuple(cases),
                title=str(meta.get("title", problem_dir.name)),
                tags=tuple(meta.get("tags", [])),
                timeout_sec=float(meta.get("timeout_sec", 2.0)),
            )
        )

    return problems
