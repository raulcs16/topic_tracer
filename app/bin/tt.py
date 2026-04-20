#!/usr/bin/env python3
import os
import re
import json
import argparse

# --- Constants ---
PROJECT_META_FILENAME = ".tt_project.json"
TOPIC_PATTERN = re.compile(r"tt\[(\w+)\]")

# --- Utility Functions ---

def find_project_root(start_path):
    """Traverse up directories to find .tt_project.json"""
    path = os.path.abspath(start_path)
    while True:
        candidate = os.path.join(path, PROJECT_META_FILENAME)
        if os.path.exists(candidate):
            return path
        parent = os.path.dirname(path)
        if parent == path:
            return None
        path = parent

def load_project_meta(project_root):
    meta_file = os.path.join(project_root, PROJECT_META_FILENAME)
    with open(meta_file, "r") as f:
        return json.load(f)

def save_project_meta(project_root, data):
    meta_file = os.path.join(project_root, PROJECT_META_FILENAME)
    with open(meta_file, "w") as f:
        json.dump(data, f, indent=4)

# --- Commands ---

def cmd_init(path=None):
    path = path or os.getcwd()
    project_name = os.path.basename(os.path.abspath(path))
    meta_file = os.path.join(path, PROJECT_META_FILENAME)

    if os.path.exists(meta_file):
        print(f"Project already initialized at {meta_file}")
        return

    project_data = {
        "name": project_name,
        "rootPath": os.path.abspath(path),
        "evidence": []
    }

    save_project_meta(path, project_data)
    print(f"Initialized project '{project_name}' at {path}")

def scan_file(file_path, project_root, existing_evidence=None):
    """Scan a single file for TT topics, skipping duplicates"""
    existing_set = set()
    if existing_evidence:
        for item in existing_evidence:
            if item["filePath"] == os.path.relpath(file_path, project_root):
                existing_set.add((item["topic"], item["lineStart"]))

    evidence_items = []
    rel_path = os.path.relpath(file_path, project_root)

    with open(file_path, "r") as f:
        lines = f.readlines()

    for idx, line in enumerate(lines):
        match = TOPIC_PATTERN.search(line)
        if match:
            topic_name = match.group(1)
            line_start = idx + 1

            if (topic_name, line_start) in existing_set:
                continue

            # Next line as signature
            signature_line = lines[idx + 1].strip() if idx + 1 < len(lines) else ""

            evidence_items.append({
                "topic": topic_name,
                "filePath": rel_path,
                "lineStart": line_start,
                "lineEnd": line_start,
                "signature": signature_line
            })

            existing_set.add((topic_name, line_start))

    return evidence_items

def cmd_scan(file_path):
    if not os.path.exists(file_path):
        print(f"File does not exist: {file_path}")
        return

    project_root = find_project_root(file_path)
    if not project_root:
        print("No TT project found in parent directories.")
        return

    project_data = load_project_meta(project_root)
    existing_evidence = project_data.get("evidence", [])

    evidence_items = scan_file(file_path, project_root, existing_evidence)
    if not evidence_items:
        print(f"No new topics found in {file_path}")
        return

    project_data.setdefault("evidence", []).extend(evidence_items)
    save_project_meta(project_root, project_data)

    print(f"Scanned {len(evidence_items)} new evidence items in {file_path}")

def cmd_list(path=None):
    """List all unique topic names in the project"""
    path = path or os.getcwd()
    project_root = find_project_root(path)
    if not project_root:
        print("No TT project found in parent directories.")
        return

    project_data = load_project_meta(project_root)
    evidence = project_data.get("evidence", [])

    topics = sorted({item["topic"] for item in evidence})
    if not topics:
        print("No topics found in project.")
        return

    print("Topics in project:")
    for topic in topics:
        print(f"- {topic}")

def cmd_list_evidence(path=None):
    """List all evidence items in the project"""
    path = path or os.getcwd()
    project_root = find_project_root(path)
    if not project_root:
        print("No TT project found in parent directories.")
        return

    project_data = load_project_meta(project_root)
    evidence = project_data.get("evidence", [])

    if not evidence:
        print("No evidence items found in project.")
        return

    print(f"Evidence items in project '{project_data.get('name', '')}':")
    for item in evidence:
        print(f"- Topic: {item['topic']}")
        print(f"  File: {item['filePath']}")
        print(f"  Line: {item['lineStart']}")
        print(f"  Signature: {item['signature']}")
        print()

def cmd_push(path=None):
    """Push local project evidence to global TopicTracer evidence.json"""
    path = path or os.getcwd()
    project_root = find_project_root(path)
    if not project_root:
        print("No TT project found in parent directories.")
        return

    project_data = load_project_meta(project_root)
    project_name = project_data.get("name")
    if not project_name:
        print("Project has no name, cannot push.")
        return

    evidence = project_data.get("evidence", [])
    if not evidence:
        print("No evidence to push.")
        return

    # Ensure the global folder exists
    global_dir = os.path.join("data")
    os.makedirs(global_dir, exist_ok=True)
    global_file_path = os.path.join(global_dir, "evidence.json")

    # Load existing global data
    if os.path.exists(global_file_path):
        with open(global_file_path, "r") as f:
            try:
                global_projects = json.load(f)
            except json.JSONDecodeError:
                global_projects = []
    else:
        global_projects = []

    # Remove existing project with same name or rootPath
    global_projects = [
        p for p in global_projects
        if p.get("name") != project_name and p.get("rootPath") != project_data.get("rootPath")
    ]

    # Append/merge the current project
    global_projects.append(project_data)

    # Save back to global file
    with open(global_file_path, "w") as f:
        json.dump(global_projects, f, indent=4)

    print(f"Pushed project '{project_name}' to {global_file_path}")


# --- CLI Entrypoint ---

def main():
    parser = argparse.ArgumentParser(description="TT CLI Tool")
    subparsers = parser.add_subparsers(dest="command")

    # init
    parser_init = subparsers.add_parser("init", help="Initialize a new TT project")
    parser_init.add_argument("path", nargs="?", default=None, help="Project root path")

    # scan
    parser_scan = subparsers.add_parser("scan", help="Scan a single file for TT topics")
    parser_scan.add_argument("file", help="Path to the file to scan")

    # list
    parser_list = subparsers.add_parser("list", help="List all unique topic names in the project")
    parser_list.add_argument("path", nargs="?", default=None, help="Project root path (optional)")

    # list-evidence
    parser_list_evidence = subparsers.add_parser(
        "list-evidence", help="List all evidence items in the project"
    )
    parser_list_evidence.add_argument(
        "path", nargs="?", default=None, help="Project root path (optional)"
    )
    # push
    parser_push = subparsers.add_parser(
        "push", help="Append local project evidence to global TopicTracer evidence.json"
    )
    parser_push.add_argument("path", nargs="?", default=None, help="Project root path (optional)")

    args = parser.parse_args()

    if args.command == "init":
        cmd_init(args.path)
    elif args.command == "scan":
        cmd_scan(args.file)
    elif args.command == "list":
        cmd_list(args.path)
    elif args.command == "list-evidence":
        cmd_list_evidence(args.path)
    elif args.command == "push":
        cmd_push(args.path)

    else:
        parser.print_help()

if __name__ == "__main__":
    main()
