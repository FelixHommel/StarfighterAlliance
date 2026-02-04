#!/usr/bin/env python3

from __future__ import annotations

import argparse
import shlex
import subprocess
from pathlib import Path


def run_command(cmd: list[str]) -> None:
    """Execute a command on the system.

    Parameters
    ----------
    cmd : list[str]
        The command that is going to be executed.
    """

    print("+", " ".join(shlex.quote(c) for c in cmd))
    subprocess.run(cmd, check=True)


def main() -> int:
    """Execute the main coverage analysis logic.

    Set up the script arguments and process the coverage data to produce refined results.

    Returns
    -------
    int
        0 if successful, anything else otherwise
    """

    parser = argparse.ArgumentParser(
        description="Generate LCOV code coverage report and optionally generate a HTML version of it."
    )
    parser.add_argument(
        "--build-dir",
        type=Path,
        default="./build/code-coverage",
        help="Build directory containing gcda/gcno files.",
    )
    parser.add_argument(
        "--out-dir",
        type=Path,
        default="./build/code-coverage/coverage-data",
        help="Output directory for report data.",
    )
    parser.add_argument(
        "--raw",
        type=str,
        default="coverage.raw.info",
        help="Raw LCOV filename (including everything).",
    )
    parser.add_argument(
        "--filtered",
        type=str,
        default="coverage.filtered.info",
        help="Filtered LCOV filename.",
    )
    parser.add_argument("--html", action="store_true", help="Generate HTML report.")
    parser.add_argument(
        "--html-dir",
        type=Path,
        default="coverage-report",
        help="HTML report output directory.",
    )
    parser.add_argument(
        "--zerocounters",
        action="store_true",
        help="Reset counters before running tests/capture.",
    )
    parser.add_argument(
        "--exclude",
        type=list[str],
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

    out = Path(args.out_dir)
    out.mkdir(parents=True, exist_ok=True)

    if args.zerocounters:
        run_command(["lcov", "--directory", str(args.build_dir), "--zerocounters"])

    # NOTE: Step 1) Gather all data
    raw = out / args.raw
    run_command(
        [
            "lcov",
            "--directory",
            str(args.build_dir),
            "--capture",
            "--ignore-errors",
            "mismatch",
            "--output-file",
            str(raw),
        ]
    )

    # NOTE: Step 2) Filter out unrelevant data
    filtered = out / args.filtered
    run_command(
        [
            "lcov",
            "--ignore-errors",
            "unused",
            "--remove",
            str(raw),
            *args.exclude,
            "--output-file",
            str(filtered),
        ]
    )

    if args.html:
        html_dir = out / args.html_dir
        html_dir.mkdir(parents=True, exist_ok=True)
        run_command(["genhtml", str(filtered), "--output-directory", str(html_dir)])
        print(f"HTML report: {html_dir.absolute()}/index.html")

    print(f"LCOV report: {filtered.absolute()}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
