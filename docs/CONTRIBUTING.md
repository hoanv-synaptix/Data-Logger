# Hướng Dẫn Đóng Góp - SynaptiX Data Logger

Chúng tôi rất hoan nghênh contributions từ cộng đồng! Tài liệu này cung cấp guidelines và best practices để đảm bảo code quality và consistent codebase.

## Mục Lục

- [Trước Khi Bắt Đầu](#trước-khi-bắt-đầu)
- [Workflow Làm Việc](#workflow-làm-việc)
- [Coding Standards](#coding-standards)
- [Testing](#testing)
- [Pull Request Process](#pull-request-process)
- [Commit Guidelines](#commit-guidelines)
- [Code Review](#code-review)
- [Reporting Issues](#reporting-issues)

---

## Trước Khi Bắt Đầu

### 1. Kiểm Tra Issues

Trước khi bắt đầu công việc mới, hãy:
- Search existing [issues](link-to-issues) để đảm bảo chưa ai làm
- Comment trên issue đó rằng bạn đang làm
- Hoặc tạo issue mới nếu đây là bug/feature mới

### 2. Đọc Tài Liệu

- [README.md](../README.md) - Project overview
- [ARCHITECTURE.md](ARCHITECTURE.md) - Kiến trúc hệ thống
- [BUILD.md](BUILD.md) - Build instructions

### 3. Setup Development Environment

Follow [BUILD.md](BUILD.md) để:
- Install toolchain
- Build firmware thành công
- Flash và verify hoạt động trên hardware

---

## Workflow Làm Việc

### Branch Strategy

```
main (protected)
  ├── feature/<feature-name>
  ├── bugfix/<issue-number>
  └── hotfix/<critical-fix>
```

**Quy tắc:**
- Never commit trực tiếp lên `main`
- Tạo branch mới cho mỗi task
- Branch names:
  - Feature: `feature/mqtt-reconnect`
  - Bugfix: `bugfix/123-mqtt-crash`
  - Hotfix: `hotfix/critical-memory-leak`

### Branch Creation

```bash
git checkout main
git pull origin main
git checkout -b feature/your-feature-name
```

### Sync với main

```bash
git fetch origin
git rebase origin/main
```

Rebase thay vì merge để giữ history clean.

---

## Coding Standards

### C Code Style

Chúng tôi tuân theo [Linux Kernel Coding Style](https://www.kernel.org/doc/html/latest/process/coding-style.html) với một số điều chỉnh:

#### 1. Indentation

- **Tabs, not spaces** (tab width = 8 spaces)
- No trailing whitespace

```c
// GOOD
void function(void)
{
    int x = 1;
    if (x) {
        do_something();
    }
}

// BAD - spaces
void function(void) {
    int x = 1;
    if (x) {
        do_something();
    }
}
```

#### 2. Braces

K&R style:

```c
if (condition) {
    action();
} else {
    other_action();
}

switch (value) {
case 1:
    do_one();
    break;
default:
    break;
}
```

#### 3. Naming

| Type | Convention | Example |
|------|------------|---------|
| Functions | `snake_case` | `void read_sensor(void)` |
| Variables | `snake_case` | `int sensor_value` |
| Constants | `UPPER_SNAKE_CASE` | `#define MAX_BUFFER 256` |
| Macros | `UPPER_SNAKE_CASE` | `#define ENABLE_FEATURE 1` |
| Structs | `snake_case_t` | `typedef struct sensor_data_t` |
| Enums | `UPPER_SNAKE_CASE` | `SENSOR_STATUS_OK` |
| Global vars | `g_` prefix | `g_system_state` |

#### 4. Comments

- **File header:** GPL/MIT license, brief description, author
- **Function comment:** Doxygen-style

```c
/**
 * @brief   Initialize sensor driver
 * @param[in]  config  Sensor configuration structure
 * @retval  0 Success
 * @retval -1 Error (invalid config)
 */
int sensor_init(const sensor_config_t *config);
```

- **Inline comments:** On separate line above code
```c
    // Check if sensor is ready
    if (status & SENSOR_READY_FLAG) {
        read_data();
    }
```

#### 5. Line Length

- **Max 80 columns** (traditional embedded)
- Break long lines at logical boundaries
- Prettier với `.clang-format` file (optional)

#### 6. Include Order

```c
/* Standard library headers */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Project headers */
#include "app_config.h"
#include "board.h"
#include "sensor.h"
```

#### 7. Error Handling

Always check return values:

```c
int ret = sensor_read(&data);
if (ret != 0) {
    LOG_ERROR("Sensor read failed: %d", ret);
    return -1;
}
```

Use `synaptix_error.h` conventions:

```c
#define SX_ERR_OK          0
#define SX_ERR_INVAL      -1
#define SX_ERR_IO         -2
#define SX_ERR_NOMEM      -3
```

---

### Doxygen Documentation

All public APIs (functions in `.h` files) MUST have Doxygen comments:

```c
/**
 * @brief   Send Modbus request
 * @details This function sends a Modbus request to the specified slave
 *          and waits for response with timeout.
 *
 * @param[in]  slave_id   Modbus slave address (1-247)
 * @param[in]  function   Modbus function code
 * @param[in]  data       Request data buffer
 * @param[in]  len        Request data length
 * @param[out] response   Response buffer (can be NULL)
 * @param[in]  timeout_ms Timeout in milliseconds
 *
 * @retval >=0 Number of bytes in response
 * @retval  -1 Invalid parameters
 * @retval  -2 Communication timeout
 * @retval  -3 CRC error
 *
 * @note This function is thread-safe when compiled with FreeRTOS enabled.
 *
 * @see modbus_response_parse()
 */
int modbus_send_request(uint8_t slave_id, uint8_t function,
                        const uint8_t *data, size_t len,
                        uint8_t *response, int timeout_ms);
```

Run Doxygen:
```bash
cd docs
doxygen Doxyfile
# Output in docs/html/
```

---

## Testing

### Unit Tests

Mỗi module nên có unit tests (với CMock, Unity, hoặc GoogleTest).

```c
// tests/sensor_test.c
#include "unity.h"
#include "sensor.h"

void setUp(void) {}
void tearDown(void) {}

void test_sensor_init_valid_config(void)
{
    sensor_config_t cfg = { .address = 0x48 };
    int ret = sensor_init(&cfg);
    TEST_ASSERT_EQUAL_INT(0, ret);
}

void test_sensor_init_null_config(void)
{
    int ret = sensor_init(NULL);
    TEST_ASSERT_EQUAL_INT(-1, ret);
}
```

### Integration Tests

Test interactions giữa modules:
- Sensor → Logger → File
- MQTT → Broker
- Modbus → External device

### Hardware Tests

Test trên real hardware với test jig hoặc manual:
- Power consumption
- Network throughput
- File system operations
- Sensor accuracy

---

## Pull Request Process

### 1. Before Submitting

- [ ] All tests pass
- [ ] Code follows style guidelines
- [ ] Doxygen comments added
- [ ] `app_config.h` updated (nếu cần)
- [ ] `config.json` updated (nếu cần)
- [ ] `CHANGELOG.md` updated
- [ ] No merge conflicts với `main`

### 2. PR Template

```markdown
## Description
[Describe changes]

## Type of Change
- [ ] Bug fix (non-breaking change which fixes an issue)
- [ ] New feature (non-breaking change which adds functionality)
- [ ] Breaking change (fix or feature that would cause existing functionality to not work as expected)
- [ ] Documentation update

## Related Issues
Closes #123
Related to #456

## Testing
- [ ] Unit tests passed
- [ ] Integration tests passed
- [ ] Hardware tested on [board name]
- Test steps: [describe steps]

## Checklist
- [ ] Code follows style guide
- [ ] Self-review completed
- [ ] No compiler warnings
- [ ] Doxygen comments added
- [ ] CHANGELOG.md updated
```

### 3. Review Process

1. **CI:** Automated builds (GCC/Clang), tests
2. **Code Review:** Ít nhất 1 reviewer approve
3. **Changes:** Address all review comments
4. **Squash:** Squash commits thành clean, logical commits
5. **Merge:** Rebase và merge vào `main`

---

## Commit Guidelines

Conventional Commits format:

```
<type>(<scope>): <subject>

<body>

<footer>
```

### Types

| Type | Mô Tả |
|------|-------|
| `feat` | New feature |
| `fix` | Bug fix |
| `docs` | Documentation changes |
| `style` | Formatting, missing semicolons, etc. (no code change) |
| `refactor` | Code restructuring |
| `test` | Adding or fixing tests |
| `chore` | Build process, tooling, etc. |
| `perf` | Performance improvement |
| `ci` | CI/CD changes |

### Examples

```bash
git commit -m "feat(mqtt): add automatic reconnect on disconnect"

git commit -m "fix(sensor): prevent NULL pointer dereference in sx_420mA_read

The sensor_read() function could crash if called with NULL config.
Add NULL check and return SX_ERR_INVAL.

Fixes #42"

git commit -m "docs(api): update Doxygen comments for modbus module

Add detailed parameter descriptions and return value documentation
for all public API functions."

git commit -m "style(board): fix indentation in board.c

Convert spaces to tabs per coding standards."
```

---

## Code Review

### For Reviewers

Khi review code:

1. **Functionality:** Code làm đúng what it claims?
2. **Design:** Có violation architecture nào không?
3. **Performance:** Có performance issue?
4. **Security:** Input validation, buffer overflows?
5. **Style:** Theo coding standards?
6. **Tests:** Có tests đầy đủ?
7. **Docs:** Comments/Doxygen updated?

### Comments Tone

- Constructive, not critical
- Ask questions: "What if X happens?" thay vì "This is wrong"
- Suggest improvements: "Consider extracting this to a function" thay vì "This is messy"

### Review Checklist

- [ ] Code builds without warnings
- [ ] No new `printf` debug statements
- [ ] No hardcoded values (use config/macros)
- [ ] Error handling adequate
- [ ] Memory management correct (no leaks)
- [ ] Thread-safe if needed (mutexes, critical sections)

---

## Reporting Issues

### Bug Report Template

```markdown
## Description
[Clear description of the bug]

## Steps to Reproduce
1. [First step]
2. [Second step]
3. [etc.]

## Expected Behavior
[What should happen]

## Actual Behavior
[What actually happens]

## Environment
- Hardware: [STM32F746 Discovery, custom board, etc.]
- Firmware version: [git commit or release tag]
- Build config: [Debug/Release, feature flags]
- Tools: [Toolchain version, IDE]

## Additional Context
- Logs/screenshots
- Related issues
```

### Feature Request

```markdown
## Problem Statement
[What problem does this solve?]

## Proposed Solution
[How should this be implemented?]

## Alternatives Considered
- [Alternative 1]
- [Alternative 2]

## Impact
- New dependencies?
- Configuration changes?
- Breaking changes?
```

---

## Communication

- **Code review:** GitHub PR comments
- **Design discussions:** GitHub Issues/Discussions
- **Urgent matters:** [Contact maintainers]

---

## Recognition

Contributors will be:
- Listed in README.md (thêm tên bạn!)
- Mentioned in release notes
- Received swag (t-shirts, stickers) cho contributions đáng kể

---

**Cảm ơn bạn đã đóng góp cho SynaptiX Data Logger! 🚀**
