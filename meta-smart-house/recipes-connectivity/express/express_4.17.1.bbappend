# initial_rev: b9b638a54fe62d1a5ba6dd6918fe4207534eb485
python do_configure_append() {
    pkgdir = d.getVar("NPM_PACKAGE")
    lockfile = os.path.join(pkgdir, "singletask.lock")
    bb.utils.remove(lockfile)
}
