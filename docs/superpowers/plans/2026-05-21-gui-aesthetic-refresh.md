# GUI Aesthetic Refresh Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Refresh the Dormora Qt Widgets GUI so it feels more polished, consistent, and presentation-ready while preserving the existing login, admin, student, persistence, and backend behavior.

**Architecture:** Keep functionality in `src/gui_main.cpp` for the first pass, but extract style constants/helpers if the edit becomes too large. Prioritize QSS, spacing, component consistency, and page layout over behavior changes. Every task must preserve the current CTest suite and produce screenshot evidence.

**Tech Stack:** C++17, Qt 6 Widgets, CMake, CTest, PowerShell on Windows.

---

## Briefing For The Worker

This project is a Qt Widgets desktop app in `E:\OOP-DormSystemProject`.

The current GUI works, but it needs visual polish:

- The dark green sidebar and Dormora identity are good and should be preserved.
- The UI feels inconsistent because cards, inputs, buttons, pills, and window controls use different radii and weights.
- The Resident Desk screen is the weakest: cramped right panel, harsh native scrollbar, clipped table text, and too many controls competing at once.
- Tables need better row height, padding, status chips, and selected/hover states.
- The student portal looks cleaner than the admin pages, but it feels like a separate product because its cream tone and yellow pills diverge too far.
- The login page is functional, but the right side has too much empty space and the card could be better composed.
- The app should remain compact and utilitarian. Do not turn it into a marketing page.

Do not remove features. Do not change backend behavior unless a GUI polish task requires a tiny helper. Keep existing tests passing.

## Files And Responsibilities

- Modify `src/gui_main.cpp`
  - Current single-file Qt Widgets UI.
  - Contains QSS in `kAppStyle`, helper widgets, page builders, action handlers, refresh methods, and test hooks.
  - Most visual changes happen here.

- Modify `CMakeLists.txt` only if new `.cpp` files are created.
  - Header-only helper extraction does not require CMake changes.

- Optionally create `src/gui/AppStyle.h`
  - Use only if extracting `kAppStyle` and visual tokens makes the implementation safer.
  - Should contain `inline constexpr const char *kAppStyle = R"(...)";`.

- Optionally create `src/gui/UiConstants.h`
  - Use only if repeated dimensions/colors become hard to maintain.
  - Prefer simple constants, not a large framework.

- Update `docs/code-review-notes.md`
  - Mark aesthetic issues addressed and list remaining design debt.

- Do not commit `build/` outputs or screenshots.

## Verification Commands

Use these commands after every task that changes code:

```powershell
$env:PATH='C:\Qt\Tools\CMake_64\bin;C:\Qt\Tools\mingw1310_64\bin;C:\Qt\6.10.2\mingw_64\bin;' + $env:PATH
cmake --build build
ctest --test-dir build --output-on-failure
```

Use these screenshot commands for visual review:

```powershell
$env:PATH='C:\Qt\Tools\CMake_64\bin;C:\Qt\Tools\mingw1310_64\bin;C:\Qt\6.10.2\mingw_64\bin;' + $env:PATH
.\build\udrms_gui.exe --screenshot build\review-login.png
.\build\udrms_gui.exe --login-admin admin --screenshot build\review-admin-dashboard.png
.\build\udrms_gui.exe --login-admin admin --page 1 --screenshot build\review-admin-residents.png
.\build\udrms_gui.exe --login-admin admin --page 2 --screenshot build\review-admin-meals.png
.\build\udrms_gui.exe --login-student S1001 --screenshot build\review-student.png
```

Expected after each verification:

- Build exits `0`.
- CTest reports `100% tests passed`.
- Screenshots are non-empty and visually show the intended page.
- No controls overlap.
- No important text is clipped.

---

## Task 1: Establish Visual Baseline

**Files:**
- Read: `src/gui_main.cpp`
- Generate only ignored screenshots under `build/`

- [ ] **Step 1: Capture current screenshots**

Run:

```powershell
$env:PATH='C:\Qt\Tools\CMake_64\bin;C:\Qt\Tools\mingw1310_64\bin;C:\Qt\6.10.2\mingw_64\bin;' + $env:PATH
.\build\udrms_gui.exe --screenshot build\before-login.png
.\build\udrms_gui.exe --login-admin admin --screenshot build\before-admin-dashboard.png
.\build\udrms_gui.exe --login-admin admin --page 1 --screenshot build\before-admin-residents.png
.\build\udrms_gui.exe --login-student S1001 --screenshot build\before-student.png
```

Expected: four PNG files are created in `build/`.

- [ ] **Step 2: Inspect the screenshots**

Look specifically for:

- clipped table text
- native scrollbars
- awkward heading wraps
- inconsistent button and pill sizes
- crowded form rows
- empty card space
- weak selected/hover/focus states

- [ ] **Step 3: Commit nothing**

This task is baseline only. Do not commit generated screenshots.

---

## Task 2: Normalize Global Style Tokens

**Files:**
- Modify: `src/gui_main.cpp`, especially `kAppStyle`

- [ ] **Step 1: Update the QSS design system**

In `kAppStyle`, normalize the visual system:

- Use one dominant card radius: `8px`.
- Use one input/button radius: `8px`.
- Use pill radius only for status pills.
- Keep the existing green brand, but add neutral gray and soft amber only for status.
- Reduce bold overload by making labels medium weight and reserving heavy weights for page titles and key metrics.

Concrete QSS targets:

```css
QLabel#pageTitle {
    font-size: 26px;
    font-weight: 800;
}
QLabel#pageKicker {
    color: #536579;
    font-size: 14px;
}
QFrame.card, QFrame[class="card"], QGroupBox {
    background: #FFFFFF;
    border: 1px solid #D9E5DF;
    border-radius: 8px;
}
QPushButton {
    background: #FFFFFF;
    border: 1px solid #D4E1DA;
    border-radius: 8px;
    padding: 9px 14px;
    font-weight: 650;
}
QLineEdit, QComboBox, QSpinBox, QDateEdit {
    background: #FFFFFF;
    border: 1px solid #D4E1DA;
    border-radius: 8px;
    padding: 8px 12px;
    min-height: 30px;
}
```

- [ ] **Step 2: Add proper disabled/focus states**

Add QSS for disabled and focus states:

```css
QPushButton:disabled, QLineEdit:disabled, QComboBox:disabled, QSpinBox:disabled {
    color: #98A2B3;
    background: #F2F5F3;
    border-color: #E2E8E4;
}
QPushButton:focus, QLineEdit:focus, QComboBox:focus, QSpinBox:focus, QDateEdit:focus {
    border: 1px solid #1D7A57;
}
```

- [ ] **Step 3: Verify**

Run:

```powershell
cmake --build build
ctest --test-dir build --output-on-failure
```

Expected: build passes and 3/3 tests pass.

- [ ] **Step 4: Screenshot check**

Capture:

```powershell
.\build\udrms_gui.exe --login-admin admin --screenshot build\style-admin-dashboard.png
```

Expected: cards, inputs, and buttons have consistent radius and focus/disabled styles do not look native.

- [ ] **Step 5: Commit**

```powershell
git add src/gui_main.cpp
git commit -m "style: normalize gui visual tokens"
```

---

## Task 3: Polish Sidebar And Window Chrome

**Files:**
- Modify: `src/gui_main.cpp`

- [ ] **Step 1: Improve window controls**

In `buildWindowControls()`, replace the visible text controls with cleaner symbols:

```cpp
auto *minimize = new QPushButton(QString::fromUtf8("−"), controls);
auto *close = new QPushButton(QString::fromUtf8("×"), controls);
```

Keep them square, aligned, and styled in QSS instead of inline style where possible.

- [ ] **Step 2: Remove inline close-button style**

Move the long `close->setStyleSheet(...)` rule into `kAppStyle`.

Expected source result:

```cpp
auto *close = new QPushButton(QString::fromUtf8("×"), controls);
close->setObjectName("closeButton");
```

- [ ] **Step 3: Tighten sidebar layout**

In `buildSidebar()` and `buildStudentPortalUi()`:

- Keep the brand top-aligned.
- Increase nav button vertical rhythm slightly.
- Replace repeated descriptive text with account/scope information.
- Ensure logout remains bottom-aligned.

- [ ] **Step 4: Verify**

Run:

```powershell
cmake --build build
ctest --test-dir build --output-on-failure
.\build\udrms_gui.exe --login-admin admin --screenshot build\chrome-sidebar-admin.png
.\build\udrms_gui.exe --login-student S1001 --screenshot build\chrome-sidebar-student.png
```

Expected:

- Window controls look intentional, not like typed characters.
- Sidebar navigation alignment is consistent.
- No sidebar text wraps awkwardly.

- [ ] **Step 5: Commit**

```powershell
git add src/gui_main.cpp
git commit -m "style: polish sidebar and window chrome"
```

---

## Task 4: Fix Tables And Data Density

**Files:**
- Modify: `src/gui_main.cpp`

- [ ] **Step 1: Improve table QSS**

Add or update table rules in `kAppStyle`:

```css
QTableWidget {
    background: #FFFFFF;
    border: 1px solid #D9E5DF;
    border-radius: 8px;
    gridline-color: transparent;
    selection-background-color: #E2F3EA;
    selection-color: #10231C;
    outline: 0;
}
QTableWidget::item {
    padding: 8px 10px;
    border-bottom: 1px solid #EEF3F0;
}
QTableWidget::item:selected {
    background: #E2F3EA;
}
QHeaderView::section {
    background: #F3F8F5;
    color: #536579;
    border: none;
    border-bottom: 1px solid #D9E5DF;
    padding: 10px;
    font-weight: 700;
}
QScrollBar:vertical {
    background: transparent;
    width: 10px;
}
QScrollBar::handle:vertical {
    background: #C7D8CF;
    border-radius: 5px;
    min-height: 32px;
}
QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
    height: 0;
}
```

- [ ] **Step 2: Improve table setup**

In `setupTable(QTableWidget *table, const QStringList &headers)`, add:

```cpp
table->verticalHeader()->setDefaultSectionSize(42);
table->setFocusPolicy(Qt::NoFocus);
table->setWordWrap(false);
```

- [ ] **Step 3: Replace plain status text with chips where feasible**

For `refreshRoomMatrix()`, use a small `QLabel` chip for the State column instead of colored plain text if practical. If not practical in the timebox, increase contrast and padding only.

- [ ] **Step 4: Verify**

Run:

```powershell
cmake --build build
ctest --test-dir build --output-on-failure
.\build\udrms_gui.exe --login-admin admin --page 1 --screenshot build\tables-residents.png
.\build\udrms_gui.exe --login-admin admin --screenshot build\tables-dashboard.png
```

Expected:

- Resident table text is not clipped.
- Native black focus rectangle is gone.
- Table row height feels intentional.
- Dashboard table is easier to scan.

- [ ] **Step 5: Commit**

```powershell
git add src/gui_main.cpp
git commit -m "style: improve gui tables"
```

---

## Task 5: Redesign Resident Desk Layout

**Files:**
- Modify: `src/gui_main.cpp`

- [ ] **Step 1: Rebalance the split layout**

In `buildStudentsPage()`, adjust split proportions so the profile panel has enough room:

```cpp
split->addWidget(buildStudentSearchCard(), 6);
split->addWidget(buildStudentProfileCard(), 5);
```

- [ ] **Step 2: Make profile panel scroll cleanly**

In `buildStudentProfileCard()`, wrap the profile content in a `QScrollArea` with `setFrameShape(QFrame::NoFrame)` and widget-resizable content. The outer card should not show a native scrollbar.

Expected structure:

```cpp
auto *box = card(this);
auto *outer = new QVBoxLayout(box);
outer->setContentsMargins(20, 18, 20, 20);
outer->addWidget(classLabel("Student Profile", "cardTitle"));

auto *scroll = new QScrollArea(box);
scroll->setWidgetResizable(true);
scroll->setFrameShape(QFrame::NoFrame);
auto *content = new QWidget(scroll);
auto *layout = new QVBoxLayout(content);
scroll->setWidget(content);
outer->addWidget(scroll, 1);
```

Move existing profile controls into `layout`.

- [ ] **Step 3: Group actions visually**

Use two action rows:

- Primary row: `Save changes`
- Secondary row: `Reset edits`, `Duplicate`, `Delete`
- Assignment row: assign/remove actions

Keep destructive delete red but less visually dominant than save.

- [ ] **Step 4: Improve Add Student card density**

Reduce visual heaviness by making the Add Student panel a plain card, not tinted as strongly as the table area. Keep it visible but secondary to “Find Student”.

- [ ] **Step 5: Verify**

Run:

```powershell
cmake --build build
ctest --test-dir build --output-on-failure
.\build\udrms_gui.exe --login-admin admin --page 1 --screenshot build\resident-desk-polished.png
```

Expected:

- No clipped controls in right panel.
- Scrollbar is styled and only appears where useful.
- Profile page feels less cramped.
- Save/delete hierarchy is clear.

- [ ] **Step 6: Commit**

```powershell
git add src/gui_main.cpp
git commit -m "style: polish resident desk layout"
```

---

## Task 6: Polish Dashboard And Meal Desk

**Files:**
- Modify: `src/gui_main.cpp`

- [ ] **Step 1: Improve metric cards**

In `addMetric()`, align metric cards with a clearer hierarchy:

- Small uppercase or medium label.
- Large value.
- One muted description.
- Consistent card height.

Keep values readable but reduce font if they dominate.

- [ ] **Step 2: Reduce empty dashboard space**

In `buildDashboardPage()`, adjust stretch factors so the room matrix does not look like a mostly empty card when only five rows exist. Options:

- reduce minimum card height
- let menu summary align closer to table height
- add a compact “available rooms” summary below the table

Do the smallest change that removes the awkward empty lower half.

- [ ] **Step 3: Improve meal cards**

In `refreshTodayMenuSummary()` and `refreshMenuCards()`:

- Make dormitory name, date, and meal lines more scannable.
- Avoid oversized pills that span the full width.
- Use consistent meal row styling.

- [ ] **Step 4: Verify**

Run:

```powershell
cmake --build build
ctest --test-dir build --output-on-failure
.\build\udrms_gui.exe --login-admin admin --screenshot build\dashboard-polished.png
.\build\udrms_gui.exe --login-admin admin --page 2 --screenshot build\meal-desk-polished.png
```

Expected:

- Dashboard looks balanced.
- Meal cards are easy to scan.
- No card feels empty or overframed.

- [ ] **Step 5: Commit**

```powershell
git add src/gui_main.cpp
git commit -m "style: polish dashboard and meal desk"
```

---

## Task 7: Polish Student Portal

**Files:**
- Modify: `src/gui_main.cpp`

- [ ] **Step 1: Bring student theme closer to admin theme**

Keep the warmer student tone, but reduce the cream/yellow dominance:

- Keep `#FFFCF7` only as a subtle background.
- Use green for brand continuity.
- Use amber for one small student-role badge only.

- [ ] **Step 2: Fix heading and badge layout**

In `pageContainer()` or `buildStudentPortalUi()`, avoid the heading wrapping awkwardly next to the badge. Put the badge after the subtitle or align it to the far right.

- [ ] **Step 3: Balance top student cards**

In `buildStudentIdentityCard()` and `buildStudentRoomCard()`:

- Make both cards use similar vertical rhythm.
- Replace the wide initials pill with a compact avatar-style chip.
- Keep resident status visible but not dominant.

- [ ] **Step 4: Improve restaurant menu section**

In the student portal menu card:

- Reduce large blank space between title and menu date.
- Use three equal meal rows.
- Keep meal initials small and consistent.

- [ ] **Step 5: Verify**

Run:

```powershell
cmake --build build
ctest --test-dir build --output-on-failure
.\build\udrms_gui.exe --login-student S1001 --screenshot build\student-portal-polished.png
```

Expected:

- Student portal feels like the same product as admin console.
- Header does not wrap awkwardly.
- Meal section has balanced spacing.

- [ ] **Step 6: Commit**

```powershell
git add src/gui_main.cpp
git commit -m "style: polish student portal"
```

---

## Task 8: Polish Login Screen

**Files:**
- Modify: `src/gui_main.cpp`

- [ ] **Step 1: Improve login composition**

In `buildLoginUi()`:

- Reduce excessive empty space on the right.
- Move the login card slightly higher or use a more intentional centered column.
- Keep the brand panel strong.

- [ ] **Step 2: Improve brand panel**

Keep text concise:

- Brand lockup
- One badge
- One headline
- One short supporting line

Avoid adding decorative blobs or gradients.

- [ ] **Step 3: Improve form rhythm**

Make labels, fields, and submit button align cleanly:

- consistent 8-10px spacing between label and field
- 16px between fields
- 20px before primary action

- [ ] **Step 4: Verify**

Run:

```powershell
cmake --build build
ctest --test-dir build --output-on-failure
.\build\udrms_gui.exe --screenshot build\login-polished.png
```

Expected:

- Login page feels balanced.
- Form does not look too low or isolated.
- No credential hint is shown.

- [ ] **Step 5: Commit**

```powershell
git add src/gui_main.cpp
git commit -m "style: polish login screen"
```

---

## Task 9: Update Review Notes And Final Visual QA

**Files:**
- Modify: `docs/code-review-notes.md`

- [ ] **Step 1: Update the review notes**

Add a section:

```markdown
## GUI Aesthetic Refresh

- Normalized card, input, button, and pill styling.
- Polished sidebar and window chrome.
- Improved table density and selected-row styling.
- Reworked Resident Desk spacing and profile scrolling.
- Balanced dashboard, meal desk, student portal, and login layouts.

Remaining design debt:

- `src/gui_main.cpp` should still be split into focused UI modules.
- Real icon assets would improve the Dormora brand mark and window controls.
- More GUI behavior tests should cover permission and form flows.
```

- [ ] **Step 2: Run full verification**

Run:

```powershell
$env:PATH='C:\Qt\Tools\CMake_64\bin;C:\Qt\Tools\mingw1310_64\bin;C:\Qt\6.10.2\mingw_64\bin;' + $env:PATH
cmake --build build
ctest --test-dir build --output-on-failure
```

Expected:

- Build exits `0`.
- CTest reports `3/3 tests passed`.

- [ ] **Step 3: Capture final screenshots**

Run:

```powershell
.\build\udrms_gui.exe --screenshot build\final-login.png
.\build\udrms_gui.exe --login-admin admin --screenshot build\final-admin-dashboard.png
.\build\udrms_gui.exe --login-admin admin --page 1 --screenshot build\final-admin-residents.png
.\build\udrms_gui.exe --login-admin admin --page 2 --screenshot build\final-admin-meals.png
.\build\udrms_gui.exe --login-admin admin --page 3 --screenshot build\final-admin-neighborhoods.png
.\build\udrms_gui.exe --login-student S1001 --screenshot build\final-student.png
```

Expected:

- All screenshots render.
- No text clipping.
- No ugly native scrollbar in the Resident Desk profile panel.
- No awkward heading wrap.
- Buttons, cards, inputs, and tables share one visual language.

- [ ] **Step 4: Commit**

```powershell
git add docs/code-review-notes.md
git commit -m "docs: record gui aesthetic refresh"
```

---

## Final Acceptance Checklist

- [ ] Existing functionality still works: admin login, student login, student add/edit/delete, room assignment, menu edit, neighborhood access.
- [ ] `cmake --build build` passes.
- [ ] `ctest --test-dir build --output-on-failure` passes with 3/3 tests.
- [ ] Final screenshots were generated and reviewed.
- [ ] No generated screenshots or `build/` artifacts are committed.
- [ ] `git status --short` only shows intentional source/doc changes before commit, and is clean after commits.

## Recommended Subagent Dispatch

Use one worker subagent for the whole plan if you want a single coherent visual direction. Use separate subagents only if each owns a disjoint slice:

- Worker A: Tasks 2-4, global style/sidebar/tables.
- Worker B: Tasks 5-8, page layouts.
- Main agent: Task 9, final review and integration.

If using multiple workers, tell them explicitly that they are not alone in the codebase and must not revert edits made by others.
