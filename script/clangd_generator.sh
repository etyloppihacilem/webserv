cat << EOF > .clangd
CompileFlags:
  Add:
    - "-Wall"
    - "-Wextra"
    - "-l$(realpath googletest/build/lib/libgtest.a)"
    - "-l$(realpath googletest/build/lib/libgmock.a)"
    - "-I$(realpath googletest/googletest/include)"
    - "-I$(realpath googletest/googlemock/include)"
    - "-I$(realpath header)"
    - "-I$(realpath src)"
    - "-std=c++98"
    - "-ferror-limit=0"

Diagnostics:
  UnusedIncludes: Strict
  MissingIncludes: Strict

---
If:
  PathMatch: .*_test\.cpp

Diagnostics:
  Suppress: access

CompileFlags:
  Add:
    - "-std=c++20"
    - "-D TESTING"

---
If:
  PathMatch: .*Test\.cpp

Diagnostics:
  Suppress: access

CompileFlags:
  Add:
    - "-std=c++20"
    - "-D TESTING"

---
If:
  PathMatch: .*test/.*\.cpp

Diagnostics:
  Suppress: access

CompileFlags:
  Add:
    - "-std=c++20"
    - "-D TESTING"

---
If:
  PathMatch: .*_test\.hpp

Diagnostics:
  Suppress: access

CompileFlags:
  Add:
    - "-std=c++20"
    - "-D TESTING"

---
If:
  PathMatch: .*Test\.hpp

Diagnostics:
  Suppress: access

CompileFlags:
  Add:
    - "-std=c++20"
    - "-D TESTING"

---
If:
  PathMatch: .*test/.*\.hpp

Diagnostics:
  Suppress: access

CompileFlags:
  Add:
    - "-std=c++20"
    - "-D TESTING"
EOF
echo Clangd correctly configured
