# -*- cmake -*-
include(Prebuilt)

if (NOT STANDALONE)
  use_prebuilt_binary(GL)
  # possible glh_linear should have its own .cmake file instead
  #use_prebuilt_binary(glh_linear)
  # actually... not any longer, it's now in git -SG
  set(GLEXT_INCLUDE_DIR ${LIBS_PREBUILT_DIR}/${LL_ARCH_DIR}/include)
endif (NOT STANDALONE)
