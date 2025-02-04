#! /usr/bin/env bash
set -e -u
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2020-25 Bradley M. Bell
# -----------------------------------------------------------------------------
# bin/new_release.sh  [--skip_stable_check_all]
# Creates and check a release for the year and release number specified below.
#
# bin/check_all.sh [--skip_external_links]
# is used by new_release.sh to check the stable branch
# correpsonding to this release (unless skipped by new_release.sh flags).
#
# bin/check_all.sh [--skip_external_links]
# is used by new_release to skip checking external links.
# new_release.sh skips this when testng before the new release (tag)  exists.
# -----------------------------------------------------------------------------
year='2025' # Year for this stable version
release='2' # first release for each year starts with 0
# -----------------------------------------------------------------------------
if [ "$0" != 'bin/new_release.sh' ]
then
   echo 'bin/new_release.sh: must be executed from its parent directory'
   exit 1
fi
if [ ! -e './.git' ]
then
   echo 'bin/new_release.sh: cannot find ./.git'
   exit 1
fi
if [[ "$year" =~ ^[0-9]{4}$ ]]
then
   echo "year = $year"
else
   echo "new_release.sh: year = $year is not valid"
   exit 1
fi
if [[ "$release" =~ ^[0-9]{1,2}$ ]]
then
   echo "release = $release"
else
   echo "new_release.sh: release = $release is not valid"
   exit 1
fi
#
# skip_stable_check_all
skip_stable_check_all='no'
while [ $# != 0 ]
do
   if [ "$1" == '--skip_stable_check_all' ]
   then
      skip_stable_check_all='yes'
   else
      echo 'bin/new_release.sh [--skip_stable_check_all]'
      echo "$1 is not a valid argument"
      exit 1
   fi
   shift
done
# -----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
   echo $*
   eval $*
}
# -----------------------------------------------------------------------------
#
# main_branch
main_branch=$(git branch --show-current)
if [ "$main_branch" != 'master' ] || [ "$main_branch" == 'main' ]
then
   echo 'bin/new_release.sh: execute using master or main branch'
   exit 1
fi
#
# sed
source bin/grep_and_sed.sh
#
# version_file_list
source bin/dev_settings.sh
#
# first_version_file
first_version_file=$(echo $version_file_list | $sed -e 's|^ *||' -e 's| .*||')
#
# version_type
cat << EOF > temp.sed
/["'][0-9]{8}["']/b one
/["'][0-9]{8}[.][0-9]{1,2}["']/b one
/["'][0-9]{4}[.][0-9]{1,2}[.][0-9]{1,2}["']/b one
b end
#
: one
s|.*["']([0-9]{8})["'].*|\\1|
s|.*["']([0-9]{8}[.][0-9]{1,2})["'].*|\\1|
s|.*["']([0-9]{4}[.][0-9]{1,2}[.][0-9]{1,2})["'].*|\\1|
p
#
: end
EOF
version=$($sed -n -r -f temp.sed $first_version_file)
if [[ "$version" =~ ^[0-9]{8}$ ]]
then
   version_type=1
elif [[ "$version" =~ ^[0-9]{8}[.][0-9]{1,2}$ ]]
then
   version_type=2
elif [[ "$version" =~ ^[0-9]{4}[.][0-9]{1,2}[.][0-9]{1,2}$ ]]
then
   version_type=3
fi
#
# tag
if [ "$version_type" == 1 ] || [ "$version_type" == 2 ]
then
   tag="${year}0000.$release"
else
   tag=$year.0.$release
fi
#
# tag_commited
tag_commited='no'
if git tag --list | grep "$tag" > /dev/null
then
   tag_commited='yes'
fi
#
# stable_branch
stable_branch=stable/$year
#
# stable_local_hash
pattern=$(echo " *refs/heads/$stable_branch" | $sed -e 's|/|[/]|g')
stable_local_hash=$(
   git show-ref $stable_branch | \
      $sed -n -e "/$pattern/p" | \
         $sed -e "s|$pattern||"
)
#
# stable_remote_hash
pattern=$(echo " *refs/remotes/origin/$stable_branch" | $sed -e 's|/|[/]|g')
stable_remote_hash=$(
   git show-ref $stable_branch | \
      $sed -n -e "/$pattern/p" | \
         $sed -e "s|$pattern||"
)
#
# main_local_hash
pattern=$(echo " *refs/heads/master" | $sed -e 's|/|[/]|g')
main_local_hash=$(
   git show-ref $main_branch | \
      $sed -n -e "/$pattern/p" | \
         $sed -e "s|$pattern||"
)
#
# main_remote_hash
pattern=$(echo " *refs/remotes/origin/master" | $sed -e 's|/|[/]|g')
main_remote_hash=$(
   git show-ref $main_branch | \
      $sed -n -e "/$pattern/p" | \
         $sed -e "s|$pattern||"
)
#
# ----------------------------------------------------------------------------
# Changes to master branch
# ----------------------------------------------------------------------------
#
# version_file_list
cat << EOF > temp.sed
s|stable-[0-9]{4}|stable-$year|g
s|release-[0-9]{4}|release-$year|g
#
s|archive/[0-9]{4}[.][0-9]*[.][0-9]*[.]tar[.]gz|archive/$tag.tar.gz|
s|archive/[0-9]{8}[.]tar[.]gz|archive/$tag.tar.gz|
s|archive/[0-9]{8}[.][0-9]*[.]tar[.]gz|archive/$tag.tar.gz|
#
s|tags/[0-9]{4}[.][0-9]*[.][0-9]*>|tags/$tag>|
s|tags/[0-9]{8}>|tags/$tag>|
s|tags/[0-9]{8}[.][0-9]*>|tags/$tag>|
#
s|tags/[0-9]{4}[.][0-9]*[.][0-9]* *\$|tags/$tag|
s|tags/[0-9]{8} *\$|tags/$tag|
s|tags/[0-9]{8}[.][0-9]* *\$|tags/$tag|
#
EOF
for file in $version_file_list
do
   $sed -r -i $file -f temp.sed
done
#
# run_xrst.sh
if [ "$tag_commited" == 'yes' ]
then
   echo_eval bin/run_xrst.sh --external_links
else
   echo_eval bin/run_xrst.sh
fi
#
# git_status
git_status=$(git status --porcelain)
if [ "$git_status" != '' ]
then
   echo "bin/new_release: git status is not empty for $main_branch branch"
   echo 'use bin/git_commit.sh to commit its changes ?'
   exit 1
fi
# ----------------------------------------------------------------------------
# Changes to stable branch
# ----------------------------------------------------------------------------
if ! git show-ref $stable_branch > /dev/null
then
   echo "bin/new_release: neither local or remvoe $stable_branch exists."
   echo 'Use the following to create it ?'
   echo "   git branch $stable_branch"
   exit 1
fi
if ! git checkout $stable_branch
then
   echo "bin/new_release: should be able to checkout $stable_branch"
   exit 1
fi
#
# version_file_list
cat << EOF > temp.sed
s|stable-[0-9]{4}|stable-$year|g
s|release-[0-9]{4}|release-$year|g
#
s|archive/[0-9]{4}[.][0-9]*[.][0-9]*[.]tar[.]gz|archive/$tag.tar.gz|
s|archive/[0-9]{8}[.]tar[.]gz|archive/$tag.tar.gz|
s|archive/[0-9]{8}[.][0-9]*[.]tar[.]gz|archive/$tag.tar.gz|
#
s|tags/[0-9]{4}[.][0-9]*[.][0-9]*>|tags/$tag>|
s|tags/[0-9]{8}>|tags/$tag>|
s|tags/[0-9]{8}[.][0-9]*>|tags/$tag>|
#
s|tags/[0-9]{4}[.][0-9]*[.][0-9]* *\$|tags/$tag|
s|tags/[0-9]{8} *\$|tags/$tag|
s|tags/[0-9]{8}[.][0-9]* *\$|tags/$tag|
#
EOF
for file in $version_file_list
do
   $sed -r -i $file -f temp.sed
done
#
# first_version_file
cat << EOF > temp.sed
s|(["'])[0-9]{8}(["'])|\\1$tag\\2|
s|(["'])[0-9]{8}[.][0-9]{1,2}(["'])|\\1$tag\\2|
s|(["'])[0-9]{4}[.][0-9]{1,2}[.][0-9]{1,2}(["'])|\\1$tag\\2|
EOF
$sed -r -f temp.sed -i $first_version_file
if ! grep "['\"]$tag['\"]" $first_version_file > /dev/null
then
   echo "bin/rew_release: branch = $stable_branch"
   echo "Version number should be $tag in $first_version_file"
   exit 1
fi
#
# check_version
# changes to version ?
if ! bin/check_version.sh
then
   echo 'Continuing even thought bin/check_version made changes.'
fi
#
# check_all.sh
if [ "$skip_stable_check_all" == 'no' ]
then
   if [ "$tag_commited" == 'yes' ]
   then
      echo_eval bin/check_all.sh --suppress_spell_warnings
   else
      echo_eval bin/check_all.sh \
         --suppress_spell_warnings --skip_external_links
   fi
fi
#
# git_status
git_status=$(git status --porcelain)
if [ "$git_status" != '' ]
then
   echo "bin/new_release: git status --porcelean not empty for $stable_branch"
   echo 'use bin/git_commit.sh to commit its changes ?'
   exit 1
fi
# -----------------------------------------------------------------------------
#
# stable_remote
if [ "$stable_remote_hash" == '' ]
then
   empty_hash='yes'
   echo "bin/new_release: remote $stable_branch does not exist."
   echo 'Use the following to create it ?'
   echo "   git push origin $stable_branch"
   exit 1
fi
if [ "$stable_local_hash" != "$stable_remote_hash" ]
then
   empty_hash='yes'
   echo "bin/new_release: local and remote $stable_branch differ."
   echo "local  $stable_local_hash"
   echo "remote $stable_remote_hash"
   echo 'Use git push to fix this ?'
   exit 1
fi
#
# push tag
if [ "$tag_commited" == 'no' ]
then
   read -p 'commit release or abort [c/a] ?' response
   if [ "$response" == 'a' ]
   then
      exit 1
   fi
   echo "git tag -a -m 'created by new_release.sh' $tag $stable_remote_hash"
   git tag -a -m 'created by new_release.sh' $tag $stable_remote_hash
   #
   echo "git push origin $tag"
   git push origin $tag
   #
   echo 'bin/new_release.sh: must be re-run to check external links'
   exit 1
fi
#
# main_remote
git checkout $main_branch
if [ "$main_local_hash" != "$main_remote_hash" ]
then
   empty_hash='yes'
   echo "bin/new_release: local and remote $main_branch differ."
   echo "local  $main_local_hash"
   echo "remote $main_remote_hash"
   echo 'Use git push to fix this ?'
   exit 1
fi
# ----------------------------------------------------------------------------
echo 'bin/new_release.sh: OK'
exit 0
