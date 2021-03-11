This subdirectory contains ConfD related documentation like 
short notes, guides, etc.

Individual documents are placed in their own subdirectories. Markdown or
Asciidoc format is used.

Documents can be viewed directly on GitHub. It's recommended to use https://github.com/zenorocha/codecopy browser extension for direct copy functionality for code blocks. 

Some Asciidoc documents can be built into `html` format with 
`gradle` script (see https://gradle.org/install/#with-a-package-manager).
Use command `gradle clean asciidoctor`. The `copy` button to is added to code blocks with `acopy` Asciidoc role (e.g. `[source,shell,role="acopy"]`). Result can be found in `build/docs` directory.

`res` subdirectory contains common resources, like styles, etc. 
