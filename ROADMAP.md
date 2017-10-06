# Roadmap

**Short-term**

- TODO: need a file descriptor stream for this...
    - fd.h
        - Getting EBADF for read/writes...
            - Maybe a fucked up swap/MOVE constructor on the streambuf?

- Need custom tempfile support
    - mkstemp?
        - Implement using file descriptor stream
    - mkdtemp?

- File descriptor streams:
    - fd_fstream
        - implementation
        - unittests
    - fd_ifstream
        - implementation
        - unittests
    - fd_ofstream
        - implementation
        - unittests

- Need string readers/writers
    - JSON
        - writer
            - implementation -- DONE
            - unittests
        - sax
            - implementation -- DONE
            - unittests
    - XML
        - writer
            - implementation -- DONE
            - unittests
        - sax
            - implementation -- DONE
            - unittests
        - writer
            - implementation
            - unittests

- Need file readers/writers
    - CSV
        - reader
            - implementation -- DONE
            - unittests
        - writer
            - implementation
            - unittests

- Add CSV reader/dictreader
    - writer
        - implementation
        - unittests
    - dict
        - implementation
        - unittests
