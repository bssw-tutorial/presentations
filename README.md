# BSSw Tutorial Presentations

The main directory is where work usually takes place to update presentations for a new tutorial. When the content is ready, PDFs are generated, renamed (to reflect the sequence of presentation) and moved to the appropriate
subdirectory of the `final-presentations` folder.  Then, the repository it tagged with the tutorial's event label, and a zip of the presentation PDFs is uploaded as part of defining a GitHub release in the repository.

**Important note:** On 2021-05-21, the template for the presentations was updated.  If you want to add slides from presentations before that date, make sure you use the *Use destination theme* option when you paste the slides in.

## Conventions

* We tag the repository when the `final-presentations` folder has been populated for a given tutorial (including PDFs).  By convention, the tags use exactly the same format as the directory names in `final-presentations`: `YYYY-MM-DD-venue`. If the nominally final slides need to be updated (i.e., to correct an error), the tag should be moved to the new version.  A zip of the PDFs (only) should be added as the release files associated with the tag.

* The names of the presentation files are important with respect to the scripting we're trying to develop.  File names here need to match those used in the tutorial web site.  That includes the `presentation.yml` and `agenda.csv` data files and the files in the `presentation-resources` directories. Everything in the web site is per-event, so presentation file names can be changed between events.

* The `presentation.yml` flle in this repository should be kept current as presentations are added or titles change.  This file is used in the bssw-tutorial website.

* Each presentation may have a supplementary `-extras` file.  The purpose of this file is to hold extra slides and variants not being used in a particular event, but which we want to keep for possible future use.
    - `-extra` file should always follow the naming of the “parent” file.  If the parent name changes, the -extra file name should change. Examples: `intro.pptx` and `intro-extra.pptx`; `overview.pptx` and `overview-extra.pptx`; etc.
    - *All* slides in the main presentation will appear in the PDF.  
        - This includes hidden slides.  In other words, they may not display if you’re presenting from the ppt, but they will appear in the PDFs we publish.
        - You’re welcome to include hidden or “backup” slides in your presentations, but consider whether they really add value.  If you just don’t want to delete them, we now have the -extra files for that.
    - Any slides you want to keep for possible future use, but don’t want to expose to tutorial participants should be put into the `-extra` file.
        - Note that the whole presentation repo is public and could be discovered by the curious.

## Presenters and Contributors

On the title page of each presentation, we list the presenter's name (underlined), pronouns, and affiliation.  We also separately list *all* of the contributors to the presentation (with short affiliations).  Every past presenter is assumed to have contributed, and others may have as well.  Contributors are listed in alphabetical order.  The current presenter is also listed as a contributor.

## Best Practices

* Minimize the use of animations.  These are often cause stumbles when presented by those not deeply familiar with the slide.  And, often, they really aren't intrinsic to the presentation.
* We **strongly** discourage the use of animations that overlay content on top of other content.  This makes the earlier layers inaccessible in static formats (i.e., PDF).

## Preparing for a Tutorial Event

The following are the general steps to follow for the production of presentations for a new tutorial event.  Some adaptation may be required for special circumstances.

1. Reserve a DOI for the event on FigShare.  The DOI will be used here, in the License, Citation, and Acknowledgment slides, and also in the bssw-tutorial website.
2. Create the `final-presentations/<event-label>` directory.
3. In the event directory, create a `doi.txt` file which contains the reserved DOI URL.
4. `git add` and `git commit` the `doi.txt` file.
5. Update the `license-master.pptx` file with the appropriate citation and any other changes that might be required.  We prefer to use the same slide in all presentations unless there are good reasons for something special.
6. Update the `agenda-master.pptx` file.  This is most easily done after adding the agenda to the event page on the website.  The agenda table on the web page can be copied and pasted into the Powerpoint table.  It works best if you create a Powerpoint table first in order to get the style from the slide template.
7. Update `intro.pptx`
   - Title slide (1) should list all presenters, then all helpers.  It should list the venue for the tutorial.
   - License slide (2) should be *replaced* with the new one from `license-master.pptx`.  This is easily done by copy and pasting in the left-hand thumbnail view, then deleting the old license slide.
   - Update the About Us slide (3) with the presenters, then helpers.  Change head shots to match.  We keep a gallery of head shots in the `head-shots` folder.
   - Update the Hands-On Activities slide (9) as appropriate for the event.  You may find some alternatives in `intro-extras.pptx`.  Copy the current slide to `-extras` before changing it.
   - Update the We Want to Interact With You slide (11) as appropriate.
8. Update the remaining presentations
   - Title slide (1) should list presenter (underlined), pronouns, and affiliation. Update tutorial venue (should be consistent across all presentaitons, including `intro.pptx`). Update contributor list if necessary. 
   - License slide (2) should be *replaced* with the new one from `license-master.pptx`
9. In the `overview.pptx` presentation, also *replace* the agenda slides at the end with the new one(s) from `agenda-master.pptx`.
10. Presenters should revise any other content as desired.
11. Update the `presentations.yml` file to reflect any new presentations or changed titles.  This file is used in the website.

## Quality Control

Once the presentations have been updated, it is always a good idea to review the slides for quality.  Slides should be viewed in presentation mode to be sure you're seeing what the audience will see.

* Confirm that the correct template is being used for all slides.
* Make sure font sizes are large enough
* Every slide should be numbered
* Look for stray or extra slide numbers.  These often come with large-scale cut-and-paste content transfers.
* Make sure that background graphics from the slide template are not unnecessarily covered (worse, partly covered) by slide content.  Often a small adjustment of size/position will allow the graphics to show as well.  If it is not possible to avoid colliding with the background graphics, consider removing them from the slide using the Format Background > Hide Background Graphics option.
* Make sure animations build in the correct order and with the appropriate elements.
* Make sure there is sufficient contrast for graphics to be intelligible.

## Preparing a Release

*Note that the bssw-tutorial website is capable of generating bash script commands that cover all but the first step in this process (though at the moment they may require a bit of adjustment here and there to match below). They are generated by the `<event-label>/utilities.md` file and can be accessed at the URL `https://bssw-tutorial.github.io/<event-label>/utilities.html` in the generated site.*

1. Foreach presentation, generate a PDF using Powerpoint's Save as Adobe PDF feature which will produce a PDF document with pages exactly sized to the template.  Do *not* print to PDF, as that will generate PDF's sized for printer paper, with sizable top and bottom margins.
3. Copy `presentations.yml` to the `final-presentations/<event-label>` directory
5. Foreach presentation PDF
   - Rename it to start with the appropriate sequence number, based on the order of the presentations (the `Module` column of the agenda table).  For example `intro.pdf` to `00-intro.pdf` and `overview.pdf` to `01-overview.pdf`.
   - Move the PDF file to the `final-presentation/<event-label>` folder.
6. `git add` and `git commit` the contents of the event directory.
7. Change to the event directory.
8. Create a zip archive of the PDF files (only) with the command `zip --update <event-label>.zip *.pdf`
   - The zip file does *not* need to be committed to the git repository since you have all of the constituent files.
9. Tag the repository for the event:
   - `git tag -a <event-label> -m "<title> at <venue>"; git push origin --tags`
10. Create a release based on the tag.  This can be done with the `gh` scripting tool:
    - `gh release create <event-label> --title "<event-date> <title> @ <venue>"; gh release upload <event-label> <event-label.zip>`

If you update any of the presentations after the release, you should update the release by moving the tag and replacing the zip file on GitHub.
