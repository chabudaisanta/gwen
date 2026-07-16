import sys
import json

def main():
    if len(sys.argv) < 2:
        print("Usage: python summary.py <result.json>")
        return
    try:
        with open(sys.argv[1], 'r', encoding='utf-8') as f:
            data = json.load(f)
    except FileNotFoundError:
        print("Result file not found.")
        return

    files = data.get('files', {})
    total = 0
    passed = 0
    failed = 0
    failed_files = []

    for filename, file_data in files.items():
        # Only count files that actually have verifications
        verifs = file_data.get('verifications', [])
        if not verifs:
            continue
            
        total += 1
        is_failed = False
        for v in verifs:
            if v.get('status') == 'failure':
                is_failed = True
                break
        
        if is_failed:
            failed += 1
            failed_files.append(filename)
        else:
            passed += 1

    print(f"Total Tests:  {total}")
    print(f"Passed (AC):  {passed}")
    print(f"Failed:       {failed}")

    if failed > 0:
        print("\n[Failed Tests]")
        for f in failed_files:
            print(f"  - {f}")

if __name__ == '__main__':
    main()
