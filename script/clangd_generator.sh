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

---
If:
  PathMatch: .*_test\.cpp

Diagnostics:
  Suppress: access

---
If:
  PathMatch: .*Test\.cpp

Diagnostics:
  Suppress: access

---
If:
  PathMatch: .*test/.*\.cpp

Diagnostics:
  Suppress: access
EOF
echo Clangd correctly configured
