file(REMOVE_RECURSE
  "libtoml11.a"
  "libtoml11.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/toml11.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
