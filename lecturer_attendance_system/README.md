# Lecturer Attendance System

A menu-driven console program for a lecturer to keep track of how many
lectures each student has attended. Records are stored in `students.txt`, one
student per line:

```
<registration number>,<department>,<lectures attended>
```

| Menu option | What it does |
| --- | --- |
| 1 Add Student | Appends a new student with 1 lecture attended |
| 2 View Student | Looks up a student by registration number |
| 3 Delete Student | Removes a student |
| 4 Edit Student | Replaces a student's details and attendance count |
| 5 View All Students | Prints every record and the total |
| 6 Mark Attendance | Adds 1 to a student's attendance count |
| 7 Exit | Quits |

Changes are written to `temporary.txt` and then renamed over `students.txt`,
so the data file is rewritten on every edit.

## Build & run

```bash
make
./lecturer_attendance_system
```

Run it from this folder so it finds `students.txt`, which ships with one
sample record.

## Known issues

- Option 5 has no `break`, so after listing the students it falls through to
  Mark Attendance and then exits.

## Compatibility

The program uses `gets()`, which was removed in C11, and calls a few functions
before declaring them. The makefile compiles with `-std=gnu99` and downgrades
the GCC 14+ / Clang 16+ errors for implicit declarations back to warnings. See
[Compatibility](../README.md#compatibility).
