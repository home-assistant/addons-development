#!/usr/bin/env bashio
set -e

PYPI=$(bashio::config 'pypi')
APK=$(bashio::config 'apk')

# Cleanup old deps
bashio::log.info "Remove old deps"
rm -rf /config/deps/*

# Need custom apk for build?
if [ -n "${APK}" ]; then
    for dep in ${APK}; do
        bashio::log.info "Installing alpine package '${dep}'"
        # shellcheck disable=SC2086
        if ! ERROR="$(apk add --no-cache ${dep})"; then
            bashio::log.error "Can't install ${dep}!"
            bashio::log.error "${ERROR}" && exit 1
        fi
    done
fi

# Install pypi modules
bashio::log.info "Install pypi modules into deps"
export PYTHONUSERBASE=/config/deps

for dep in ${PYPI}; do
    bashio::log.info "Installing python package '${dep}'"
    
    # shellcheck disable=SC2086
    if ! ERROR="$(pip3 install --user --no-cache-dir --prefix= --no-dependencies --disable-pip-version-check ${dep})"; then
        bashio::log.error "Can't install ${dep}!"
        bashio::log.error "${ERROR}" && exit 1
    fi
done

bashio::log.info "Done"
