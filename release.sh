#!/bin/bash

set -e

echo "Login keys available?"
if [ -z "$CLOUDFLARE_BLACKWATCH_KEY" ]; then
  echo "Error: CLOUDFLARE_BLACKWATCH_KEY is not set." >&2
  exit 1
fi

sign_installer() {
    local item=$1
    local target=$2
    echo "Signing $item..."
    productsign --sign "Developer ID Installer: All Star Apps, LLC (768Z86F8ET)" "$item" "$target"
    echo "Verifying pkg sig..."
    pkgutil --check-signature "$target"
}

sign_file() {
    local item=$1
    echo "Signing $item..."
    codesign --entitlements ./entitlements.plist --deep --force --verbose --options runtime --sign "Developer ID Application: All Star Apps, LLC (768Z86F8ET)" "$item"
    echo "Verifying signature..."
    codesign --verify --verbose "$item"
}

#read -p "Notarize? (y/n) " answer && do_notarize=$([[ $answer == "y" ]] && echo true || echo false)
do_notarize=true

function notarize_app() {
  if ! $do_notarize; then
    echo "Skip notarization..."
    return;
  fi

    local APP_PATH="$1"
    local APPLE_ID="allstarappsllc@gmail.com"
    local TEAM_ID="768Z86F8ET"
    local PASSWORD_LABEL="SD Notary 2"

    # Store credentials (run this part once manually to store your app-specific password)
    if false; then
      echo "Notarization credentials..."
      xcrun notarytool store-credentials "$PASSWORD_LABEL" --apple-id "$APPLE_ID" --team-id "$TEAM_ID"
    fi

    # Notarize the application
    echo "Notarizing..."
    local notarization_info=$(xcrun notarytool submit "$APP_PATH" --keychain-profile "SD Notary 2" --wait)

    # Check the result of notarization
    if [[ $notarization_info == *"Current status: Accepted"* ]]; then
      echo "Stapling..."
      xcrun stapler staple "$APP_PATH"

      echo "Notarization successful."
    else
      echo "$notarization_info"
      echo ""
      echo "For details, run:"
      echo ""
      echo "xcrun notarytool log --keychain-profile \"SD Notary 2\" SUBMISSIONIDGOESHERE"
      echo ""
      echo "Notarization failed."
      exit 1
    fi
}

echo "Clear out dir..."
rm -rf out/*

echo "Run pre-build..."
./plugin/pre-build.sh
cd "$(dirname "$0")"

echo "Setup universal build..."
mkdir -p out/plugins/arm64
cmake -S . -B out/plugins/arm64  -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES="arm64" # x86_64;arm64

echo "Building plugins..."
cmake --build ./out/plugins/arm64 -j 6 --config Release

echo "Sign AU..."
sign_file out/plugins/arm64/plugin/BlackWatchPlugin_artefacts/Release/AU/BlackwatchPlugin.component

echo "Sign VST3..."
sign_file out/plugins/arm64/plugin/BlackWatchPlugin_artefacts/Release/VST3/BlackwatchPlugin.vst3

echo "Build AU pkg..."
mkdir -p out/temp_installers/au_root
mkdir -p out/temp_installers/unsigned_pkgs
mkdir -p out/temp_installers/signed_pkgs
cp -R out/plugins/arm64/plugin/BlackWatchPlugin_artefacts/Release/AU/BlackwatchPlugin.component out/temp_installers/au_root/BlackwatchPlugin.component
pkgbuild --root out/temp_installers/au_root \
         --identifier com.allstarapps.blackwatchau \
         --version 1.0.0 \
         --install-location /Library/Audio/Plug-Ins/Components \
         out/temp_installers/unsigned_pkgs/au.pkg
sign_installer out/temp_installers/unsigned_pkgs/au.pkg out/temp_installers/signed_pkgs/au.pkg

echo "Build VST3 pkg..."
mkdir -p out/temp_installers/vst_root
cp -R out/plugins/arm64/plugin/BlackWatchPlugin_artefacts/Release/VST3/BlackwatchPlugin.vst3 out/temp_installers/vst_root/BlackwatchPlugin.vst3
pkgbuild --root out/temp_installers/vst_root \
         --identifier com.allstarapps.blackwatchvst3 \
         --version 1.0.0 \
         --install-location /Library/Audio/Plug-Ins/VST3 \
         out/temp_installers/unsigned_pkgs/vst3.pkg
sign_installer out/temp_installers/unsigned_pkgs/vst3.pkg out/temp_installers/signed_pkgs/vst3.pkg


if true; then
echo "Build wavs pkg..."
rm -rf "/Library/Application Support/BlackwatchStudiosDevelopment/pkgs"
mkdir -p "/Library/Application Support/BlackwatchStudiosDevelopment/pkgs/unsigned"
mkdir -p "/Library/Application Support/BlackwatchStudiosDevelopment/pkgs/signed"
pkgbuild --root "/Library/Application Support/BlackwatchStudiosDevelopment/wavs" \
         --install-location "/Library/Application Support/BlackwatchStudios/wavs" \
         --identifier com.allstarapps.blackwatchpluginwavs \
         --version 1.0 \
         "/Library/Application Support/BlackwatchStudiosDevelopment/pkgs/unsigned/wavs.pkg"
echo "Sign wavs.pkg..."
sign_installer "/Library/Application Support/BlackwatchStudiosDevelopment/pkgs/unsigned/wavs.pkg" "/Library/Application Support/BlackwatchStudiosDevelopment/pkgs/signed/wavs.pkg"
fi

echo "Combine installers..."
mkdir -p out/installers
productbuild --distribution productbuild.xml \
             --package-path out/temp_installers/signed_pkgs \
             --package-path "/Library/Application Support/BlackwatchStudiosDevelopment/pkgs/signed" \
             --sign "Developer ID Installer: All Star Apps, LLC (768Z86F8ET)" \
             out/temp_installers/InstallBlackwatchUnsigned.pkg
sign_installer out/temp_installers/InstallBlackwatchUnsigned.pkg out/installers/InstallBlackwatch.pkg

echo "Notarize..."
notarize_app out/installers/InstallBlackwatch.pkg


echo "Uploading..."
export RCLONE_CONFIG_MYR2_TYPE=s3
export RCLONE_CONFIG_MYR2_PROVIDER=Cloudflare
export RCLONE_CONFIG_MYR2_ACCESS_KEY_ID="$CLOUDFLARE_BLACKWATCH_KEY"
export RCLONE_CONFIG_MYR2_SECRET_ACCESS_KEY="$CLOUDFLARE_BLACKWATCH_SECRET"
export RCLONE_CONFIG_MYR2_ENDPOINT="$CLOUDFLARE_BLACKWATCH_ENDPOINT"
rclone copyto --progress ./out/installers/InstallBlackwatch.pkg MYR2:blackwatch/installers/InstallBlackwatch_Mac_MSeries.pkg

echo ""
echo "Download from:"
echo " • https://blackwatchplugin.com/installers/InstallBlackwatch_Mac_MSeries.pkg"

echo "Success!"
exit 0