#!/bin/sh

set -e

downloadFolder="${TMPDIR:-/tmp}"
downloaded_file="${downloadFolder}/pvm"
executable_folder="/usr/local/bin"
exe_name="pvm"
asset_uri="https://github.com/ardagnsrn/pvm-ubuntu/releases/latest/download/${exe_name}"

mkdir -p "${downloadFolder}"

echo "[1/3] Download ${asset_uri} to ${downloadFolder}"
rm -f "${downloaded_file}"
curl --fail --location --output "${downloaded_file}" "${asset_uri}"

echo "[2/3] Install PVM to the ${executable_folder}"
rm -f ${executable_folder}/${exe_name}
mv "${downloaded_file}" ${executable_folder}
exe=${executable_folder}/${exe_name}
chmod +x ${exe}

echo "[3/3] Set environment variables"
echo "PHP Version Manager (pvm) is installed successfully!"
if command -v $exe_name >/dev/null; then
    echo "Run '$exe_name' to get started."
else
    echo "Manually add the directory to your \$HOME/.bash_profile (or similar)"
    echo "  export PATH=${executable_folder}:\$PATH"
    echo "Run '$exe_name' to get started."
fi

exit 0