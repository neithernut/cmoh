# we build the user documentation using pandoc
find_program(PANDOC pandoc)

if(PANDOC)
    list(APPEND DOC_SOURCES
        Introduction.md
        Motivation.md
        Usage.md
        Properties.md
        Factories.md
        AccessorBundle.md
        Utilities.md
        Installation.md
        Licensing.md
        metadata.yaml
    )
    add_custom_target(pdfdoc
        pandoc -o userdoc.pdf --toc --chapters ${DOC_SOURCES}
        DEPENDS ${DOC_SOURCES}
    )
else(PANDOC)
    message("pandoc not found")
endif(PANDOC)

