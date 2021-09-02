# BSSw Tutorial Presentations

The main directory is where work usually takes place to update presentations for a new tutorial. When the content is ready, *copies* are moved to the appropriate subdirectory of `final-presentations` and PDFs are generated.

**Important note:** On 2021-05-21, the template for the presentations was updated.  If you want to add slides from presentations before that date, make sure you use the *Use destination theme* option when you paste the slides in.

## Conventions

* We tag the repository when the `final-presentations` folder has been populated for a given tutorial (including PDFs).  By convention, the tags use exactly the same format as the directory names in `final-presentations`: `YYYY-MM-DD-venue`. If the nominally final slides need to be updated (i.e., to correct an error), the tag should be moved to the new version.  A zip of the PDFs (only) should be added as the release files associated with the tag.

* The names of the presentation files are important with respect to the scripting we're trying to develop.  File names here need to match those used in the tutorial web site.  That includes the `presentation.yml` and `agenda.csv` data files and the files in the `presentation-resources` directories. Everything in the web site is per-event, so presentation file names can be changed between events.

* Each presentation may have a supplementary `-extras` file.  The purpose of this file is to hold extra slides and variants not being used in a particular event, but which we want to keep for possible future use.
    - `-extra` file should always follow the naming of the “parent” file.  If the parent name changes, the -extra file name should change. Examples: `intro.pptx` and `intro-extra.pptx`; `overview.pptx` and `overview-extra.pptx`; etc.
    - *All* slides in the main presentation will appear in the PDF.  
        - This includes hidden slides.  In other words, they may not display if you’re presenting from the ppt, but they will appear in the PDFs we publish.
        - You’re welcome to include hidden or “backup” slides in your presentations, but consider whether they really add value.  If you just don’t want to delete them, we now have the -extra files for that.
    - Any slides you want to keep for possible future use, but don’t want to expose to tutorial participants should be put into the `-extra` file.
        - Note that the whole presentation repo is public and could be discovered by the curious.
