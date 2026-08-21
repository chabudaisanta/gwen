#!/usr/bin/env python3
"""公開ドキュメントの目次とヘッダー仕様書の対応を検査する。"""

from __future__ import annotations

import re
import sys
from collections import Counter
from pathlib import Path


ROOT = Path(__file__).resolve().parent.parent
INDEX = ROOT / ".verify-helper/docs/index.md"
HEADER_ROOT = ROOT / "include/gwen"
DOC_ROOT = ROOT / "doc"
LINK_PATTERN = re.compile(r"\[[^]]+\]\(([^)]+)\)")
DOC_TARGET_PATTERN = re.compile(r"^documentation_of:\s*//(include/gwen/.+\.hpp)\s*$", re.MULTILINE)
FRONT_MATTER_PATTERN = re.compile(r"\A---\s*\n(.*?)\n---\s*(?:\n|\Z)", re.DOTALL)


def main() -> int:
    errors: list[str] = []
    index_links = LINK_PATTERN.findall(INDEX.read_text(encoding="utf-8"))
    header_links = [link for link in index_links if link.startswith("include/gwen/")]
    local_links = {link for link in index_links if "://" not in link and not link.startswith("#")}

    for link in index_links:
        if "://" in link or link.startswith("#"):
            continue
        if not (ROOT / link).is_file():
            errors.append(f"目次のリンク先が存在しません: {link}")

    linked_header_paths = [Path(link).relative_to("include/gwen") for link in header_links]
    linked_headers = set(linked_header_paths)
    for header, count in sorted(Counter(linked_header_paths).items()):
        if count > 1:
            errors.append(f"公開ヘッダーが目次に重複掲載されています: include/gwen/{header}")
    headers = {path.relative_to(HEADER_ROOT) for path in HEADER_ROOT.rglob("*.hpp")}
    for header in sorted(headers - linked_headers):
        errors.append(f"目次に未掲載の公開ヘッダーです: include/gwen/{header}")
    for header in sorted(linked_headers - headers):
        errors.append(f"目次が存在しない公開ヘッダーを指しています: include/gwen/{header}")

    documented_headers: list[Path] = []
    for document in DOC_ROOT.rglob("*.md"):
        front_matter = FRONT_MATTER_PATTERN.match(document.read_text(encoding="utf-8"))
        match = DOC_TARGET_PATTERN.search(front_matter.group(1)) if front_matter is not None else None
        if match is None:
            document_link = document.relative_to(ROOT).as_posix()
            if document_link not in local_links:
                errors.append(f"開発用資料が目次に未掲載です: {document_link}")
            continue
        header = Path(match.group(1)).relative_to("include/gwen")
        documented_headers.append(header)
        expected_document = DOC_ROOT / header.with_suffix(".md")
        if document != expected_document:
            errors.append(
                "仕様書の配置がヘッダーと対応していません: "
                f"{document.relative_to(ROOT)} (期待値: {expected_document.relative_to(ROOT)})"
            )
    documented_header_set = set(documented_headers)
    for header, count in sorted(Counter(documented_headers).items()):
        if count > 1:
            errors.append(f"公開ヘッダーに複数の仕様書があります: include/gwen/{header}")
    for header in sorted(headers - documented_header_set):
        errors.append(f"仕様書がない公開ヘッダーです: include/gwen/{header}")
    for header in sorted(documented_header_set - headers):
        errors.append(f"仕様書が存在しない公開ヘッダーを指しています: include/gwen/{header}")

    if errors:
        print("公開ドキュメントの整合性チェックに失敗しました:", file=sys.stderr)
        for error in errors:
            print(f"- {error}", file=sys.stderr)
        return 1

    print("公開ドキュメントの整合性チェックに成功しました。")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
