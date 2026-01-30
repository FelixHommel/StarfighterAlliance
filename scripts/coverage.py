#!/usr/bin/env python3

from __future__ import annotations

import argparse
import shlex
import subprocess
from pathlib import Path


def run_command(cmd: list[str]) -> None:
    print("+", " ".join(shlex.quote(c) for c in cmd))
    subprocess.run(cmd, check=True)


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Generate LCOV and optional HTML coverage reports."
    )
    parser.add_argument(
        "--build-dir", type=Path, default="build/code-coverage", help=""
    )
    parser.add_argument(
        "--out-dir", type=Path, default="build/code-coverage/coverage-data", help=""
    )
    parser.add_argument("--raw", default="coverage.raw.info", help="")
    parser.add_argument("--filtered", default="coverage.filtered.info", help="")
    parser.add_argument("--html", action="store_true", help="")
    parser.add_argument("--html-dir", type=Path, default="coverage-report", help="")
    parser.add_argument("--zerocounters", action="store_true", help="")
    parser.add_argument(
        "--exclude",
        action="append",
        default=[
            "/usr/*",
            "*/build/*",
            "*/vcpkg_installed/*",
            "*/src/test/*",
            "*/src/app/*",
        ],
        help="Which directories to exclude from analysis",
    )
    args = parser.parse_args()

    args.out_dir.mkdir(parents=True, exist_ok=True)

    if args.zerocounters:
        run_command(["lcov", "--directory", str(args.build_dir), "--zerocounters"])

    raw = args.out_dir / args.raw
    run_command([
        "lcov",
        "--directory", str(args.build_dir),
        "--capture",
        "--ignore-errors", "mismatch",
        "--output-file", str(raw)
    ])
    filtered = args.out_dir / args.filtered
    run_command([
        "lcov",
        "--ignore-errors", "unused"
        "--remove", str(raw), *args.exclude,
        "--output-file", str(filtered)
    ])

    if args.html:
        html_dir = args.out_dir / args.html_dir
        html_dir.mkdir(parents=True, exist_ok=True)
        run_command([
            "genhtml", str(filtered),
            "--output-directory", str(html_dir)
        ])
        print(f"HTML report: {html_dir}/index.html")

    print(f"LCOV report: {filtered}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
