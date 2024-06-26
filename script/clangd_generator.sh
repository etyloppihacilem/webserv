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

---
If:
  PathMatch: .*Test\.cpp

Diagnostics:
  Suppress: access

CompileFlags:
  Add:
    - "-std=c++20"

---
If:
  PathMatch: .*test/.*\.cpp

Diagnostics:
  Suppress: access

CompileFlags:
  Add:
    - "-std=c++20"
EOF
echo Clangd correctly configured
