Release Process
====================

* Update translations (ping wumpus, Diapolo or tcatm on IRC) see [translation_process.md](https://github.com/iranbit/iranbit/blob/master/doc/translation_process.md#syncing-with-transifex)
* Update [bips.md](bips.md) to account for changes since the last release.
* Update hardcoded [seeds](/contrib/seeds)

* * *

###First time / New builders
Check out the source code in the following directory hierarchy.

	cd /path/to/your/toplevel/build
	git clone https://github.com/iranbit/gitian.sigs.git
	git clone https://github.com/iranbit/iranbit-detached-sigs.git
	git clone https://github.com/devrandom/gitian-builder.git
	git clone https://github.com/iranbit/iranbit.git

###Iranbit maintainers/release engineers, update (commit) version in sources

	pushd ./iranbit
	contrib/verifysfbinaries/verify.sh
	configure.ac
	doc/README*
	doc/Doxyfile
	contrib/gitian-descriptors/*.yml
	src/clientversion.h (change CLIENT_VERSION_IS_RELEASE to true)

	# tag version in git

	git tag -s v(new version, e.g. 0.8.0)

	# write release notes. git shortlog helps a lot, for example:

	git shortlog --no-merges v(current version, e.g. 0.7.2)..v(new version, e.g. 0.8.0)
	popd

* * *

###Setup and perform Gitian builds

 Setup Gitian descriptors:

	pushd ./iranbit
	export SIGNER=(your Gitian key, ie bluematt, sipa, etc)
	export VERSION=(new version, e.g. 0.8.0)
	git fetch
	git checkout v${VERSION}
	popd

  Ensure your gitian.sigs are up-to-date if you wish to gverify your builds against other Gitian signatures.

	pushd ./gitian.sigs
	git pull
	popd

  Ensure gitian-builder is up-to-date to take advantage of new caching features (`e9741525c` or later is recommended).

	pushd ./gitian-builder
	git pull

###Fetch and create inputs: (first time, or when dependency versions change)

	mkdir -p inputs
	wget -P inputs https://iranbitcore.org/cfields/osslsigncode-Backports-to-1.7.1.patch
	wget -P inputs http://downloads.sourceforge.net/project/osslsigncode/osslsigncode/osslsigncode-1.7.1.tar.gz

 Register and download the Apple SDK: see [OS X readme](README_osx.txt) for details.

 https://developer.apple.com/devcenter/download.action?path=/Developer_Tools/xcode_6.1.1/xcode_6.1.1.dmg

 Using a Mac, create a tarball for the 10.9 SDK and copy it to the inputs directory:

	tar -C /Volumes/Xcode/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/ -czf MacOSX10.9.sdk.tar.gz MacOSX10.9.sdk

###Optional: Seed the Gitian sources cache and offline git repositories

By default, Gitian will fetch source files as needed. To cache them ahead of time:

	make -C ../iranbit/depends download SOURCES_PATH=`pwd`/cache/common

Only missing files will be fetched, so this is safe to re-run for each build.

NOTE: Offline builds must use the --url flag to ensure Gitian fetches only from local URLs. For example:
```
./bin/gbuild --url iranbit=/path/to/iranbit,signature=/path/to/sigs {rest of arguments}
```
The gbuild invocations below <b>DO NOT DO THIS</b> by default.

###Build and sign Iranbit Core for Linux, Windows, and OS X:

	./bin/gbuild --commit iranbit=v${VERSION} ../iranbit/contrib/gitian-descriptors/gitian-linux.yml
	./bin/gsign --signer $SIGNER --release ${VERSION}-linux --destination ../gitian.sigs/ ../iranbit/contrib/gitian-descriptors/gitian-linux.yml
    mv build/out/iranbit-*.tar.gz build/out/src/iranbit-*.tar.gz ../

	./bin/gbuild --commit iranbit=v${VERSION} ../iranbit/contrib/gitian-descriptors/gitian-win.yml
	./bin/gsign --signer $SIGNER --release ${VERSION}-win-unsigned --destination ../gitian.sigs/ ../iranbit/contrib/gitian-descriptors/gitian-win.yml
    mv build/out/iranbit-*-win-unsigned.tar.gz inputs/iranbit-win-unsigned.tar.gz
    mv build/out/iranbit-*.zip build/out/iranbit-*.exe ../

	./bin/gbuild --commit iranbit=v${VERSION} ../iranbit/contrib/gitian-descriptors/gitian-osx.yml
	./bin/gsign --signer $SIGNER --release ${VERSION}-osx-unsigned --destination ../gitian.sigs/ ../iranbit/contrib/gitian-descriptors/gitian-osx.yml
    mv build/out/iranbit-*-osx-unsigned.tar.gz inputs/iranbit-osx-unsigned.tar.gz
    mv build/out/iranbit-*.tar.gz build/out/iranbit-*.dmg ../

  Build output expected:

  1. source tarball (iranbit-${VERSION}.tar.gz)
  2. linux 32-bit and 64-bit dist tarballs (iranbit-${VERSION}-linux[32|64].tar.gz)
  3. windows 32-bit and 64-bit unsigned installers and dist zips (iranbit-${VERSION}-win[32|64]-setup-unsigned.exe, iranbit-${VERSION}-win[32|64].zip)
  4. OS X unsigned installer and dist tarball (iranbit-${VERSION}-osx-unsigned.dmg, iranbit-${VERSION}-osx64.tar.gz)
  5. Gitian signatures (in gitian.sigs/${VERSION}-<linux|{win,osx}-unsigned>/(your Gitian key)/

###Verify other gitian builders signatures to your own. (Optional)

  Add other gitian builders keys to your gpg keyring

	gpg --import ../iranbit/contrib/gitian-downloader/*.pgp

  Verify the signatures

	./bin/gverify -v -d ../gitian.sigs/ -r ${VERSION}-linux ../iranbit/contrib/gitian-descriptors/gitian-linux.yml
	./bin/gverify -v -d ../gitian.sigs/ -r ${VERSION}-win-unsigned ../iranbit/contrib/gitian-descriptors/gitian-win.yml
	./bin/gverify -v -d ../gitian.sigs/ -r ${VERSION}-osx-unsigned ../iranbit/contrib/gitian-descriptors/gitian-osx.yml

	popd

###Next steps:

Commit your signature to gitian.sigs:

	pushd gitian.sigs
	git add ${VERSION}-linux/${SIGNER}
	git add ${VERSION}-win-unsigned/${SIGNER}
	git add ${VERSION}-osx-unsigned/${SIGNER}
	git commit -a
	git push  # Assuming you can push to the gitian.sigs tree
	popd

  Wait for Windows/OS X detached signatures:
	Once the Windows/OS X builds each have 3 matching signatures, they will be signed with their respective release keys.
	Detached signatures will then be committed to the [iranbit-detached-sigs](https://github.com/iranbit/iranbit-detached-sigs) repository, which can be combined with the unsigned apps to create signed binaries.

  Create (and optionally verify) the signed OS X binary:

	pushd ./gitian-builder
	./bin/gbuild -i --commit signature=v${VERSION} ../iranbit/contrib/gitian-descriptors/gitian-osx-signer.yml
	./bin/gsign --signer $SIGNER --release ${VERSION}-osx-signed --destination ../gitian.sigs/ ../iranbit/contrib/gitian-descriptors/gitian-osx-signer.yml
	./bin/gverify -v -d ../gitian.sigs/ -r ${VERSION}-osx-signed ../iranbit/contrib/gitian-descriptors/gitian-osx-signer.yml
	mv build/out/iranbit-osx-signed.dmg ../iranbit-${VERSION}-osx.dmg
	popd

  Create (and optionally verify) the signed Windows binaries:

	pushd ./gitian-builder
	./bin/gbuild -i --commit signature=v${VERSION} ../iranbit/contrib/gitian-descriptors/gitian-win-signer.yml
	./bin/gsign --signer $SIGNER --release ${VERSION}-win-signed --destination ../gitian.sigs/ ../iranbit/contrib/gitian-descriptors/gitian-win-signer.yml
	./bin/gverify -v -d ../gitian.sigs/ -r ${VERSION}-win-signed ../iranbit/contrib/gitian-descriptors/gitian-win-signer.yml
	mv build/out/iranbit-*win64-setup.exe ../iranbit-${VERSION}-win64-setup.exe
	mv build/out/iranbit-*win32-setup.exe ../iranbit-${VERSION}-win32-setup.exe
	popd

Commit your signature for the signed OS X/Windows binaries:

	pushd gitian.sigs
	git add ${VERSION}-osx-signed/${SIGNER}
	git add ${VERSION}-win-signed/${SIGNER}
	git commit -a
	git push  # Assuming you can push to the gitian.sigs tree
	popd

-------------------------------------------------------------------------

### After 3 or more people have gitian-built and their results match:

- Create `SHA256SUMS.asc` for the builds, and GPG-sign it:
```bash
sha256sum * > SHA256SUMS
gpg --digest-algo sha256 --clearsign SHA256SUMS # outputs SHA256SUMS.asc
rm SHA256SUMS
```
(the digest algorithm is forced to sha256 to avoid confusion of the `Hash:` header that GPG adds with the SHA256 used for the files)
Note: check that SHA256SUMS itself doesn't end up in SHA256SUMS, which is a spurious/nonsensical entry.

- Upload zips and installers, as well as `SHA256SUMS.asc` from last step, to the iranbit.org server
  into `/var/www/bin/iranbit-core-${VERSION}`

- Update iranbit.org version

  - First, check to see if the Iranbit.org maintainers have prepared a
    release: https://github.com/iranbit-dot-org/iranbit.org/labels/Releases

      - If they have, it will have previously failed their Travis CI
        checks because the final release files weren't uploaded.
        Trigger a Travis CI rebuild---if it passes, merge.

  - If they have not prepared a release, follow the Iranbit.org release
    instructions: https://github.com/iranbit-dot-org/iranbit.org#release-notes

  - After the pull request is merged, the website will automatically show the newest version within 15 minutes, as well
    as update the OS download links. Ping @saivann/@harding (saivann/harding on Freenode) in case anything goes wrong

- Announce the release:

  - Release sticky on iranbittalk: https://iranbittalk.org/index.php?board=1.0

  - Iranbit-development mailing list

  - Update title of #iranbit on Freenode IRC

  - Optionally reddit /r/Iranbit, ... but this will usually sort out itself

- Notify BlueMatt so that he can start building [the PPAs](https://launchpad.net/~iranbit/+archive/ubuntu/iranbit)

- Add release notes for the new version to the directory `doc/release-notes` in git master

- Celebrate
