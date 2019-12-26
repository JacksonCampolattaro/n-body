/*
    This file is part of Magnum.

    Copyright © 2010, 2011, 2012, 2013, 2014, 2015, 2016, 2017, 2018, 2019
              Vladimír Vondruš <mosra@centrum.cz>

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

#include <Corrade/Containers/Optional.h>
#include <Corrade/Containers/StaticArray.h>
#include <Corrade/PluginManager/Manager.h>
#include <Corrade/Utility/Arguments.h>
#include <Corrade/Utility/ConfigurationGroup.h>
#include <Corrade/Utility/DebugStl.h>
#include <Corrade/Utility/Directory.h>
#include <Corrade/Utility/String.h>

#include "Magnum/PixelFormat.h"
#include "Magnum/Trade/AbstractImporter.h"
#include "Magnum/Trade/AbstractImageConverter.h"
#include "Magnum/Trade/ImageData.h"

namespace Magnum {

/** @page magnum-imageconverter Image conversion utility
@brief Converts images of different formats

@m_footernavigation
@m_keywords{magnum-imageconverter imageconverter}

This utility is built if both `WITH_TRADE` and `WITH_IMAGECONVERTER` is enabled
when building Magnum. To use this utility with CMake, you need to request the
`imageconverter` component of the `Magnum` package and use the
`Magnum::imageconverter` target for example in a custom command:

@code{.cmake}
find_package(Magnum REQUIRED imageconverter)

add_custom_command(OUTPUT ... COMMAND Magnum::imageconverter ...)
@endcode

See @ref building, @ref cmake and the @ref Trade namespace for more
information.

@section magnum-imageconverter-usage Usage

@code{.sh}
magnum-imageconverter [-h|--help] [--importer IMPORTER] [--converter CONVERTER]
    [--plugin-dir DIR] [--] input output
@endcode

Arguments:

-   `input` --- input image
-   `output` --- output image
-   `-h`, `--help` --- display this help message and exit
-   `--importer IMPORTER` --- image importer plugin (default:
    @ref Trade::AnyImageImporter "AnyImageImporter")
-   `--converter CONVERTER` --- image converter plugin (default:
    @ref Trade::AnyImageConverter "AnyImageConverter")
-   `--plugin-dir DIR` --- override base plugin dir
-   `-i`, `--importer-options key=val,key2=val2,…` --- configuration options to
    pass to the importer
-   `-c`, `--converter-options key=val,key2=val2,…` --- configuration options
    to pass to the converter

Specifying `--importer raw:&lt;format&gt;` will treat the input as a raw
tightly-packed square of pixels in given @ref PixelFormat. Specifying
`--converter raw` will save raw imported data instead of using a converter
plugin.

The `-i` / `--importer-options` and `-c` / `--converter-options` arguments
accept a comma-separated list of key/value pairs to set in the importer /
converter plugin configuration. If the `=` character is omitted, it's
equivalent to saying `key=true`.

@section magnum-imageconverter-example Example usage

Converting a JPEG file to a PNG:

@code{.sh}
magnum-imageconverter image.jpg image.png
@endcode

Creating a JPEG file with 95% quality from a PNG, by setting a
@ref Trade-JpegImageConverter-configuration "plugin-specific configuration option".
Note that currently the proxy @ref Trade::AnyImageImporter "AnyImageImporter"
and @ref Trade::AnyImageConverter "AnyImageConverter" plugins don't know how to
correctly propagate options to the target plugin, so you need to specify
`--importer` / `--converter` explicitly when using the `-i` / `-c` options.

@m_class{m-console-wrap}

@code{.sh}
magnum-imageconverter image.png image.jpg -c jpegQuality=0.95 --converter JpegImageConverter
@endcode

Extracting raw (uncompressed, compressed) data from a DDS file for manual
inspection:

@code{.sh}
magnum-imageconverter image.dds --converter raw data.dat
@endcode
*/

}

using namespace Magnum;

namespace {

void setOptions(PluginManager::AbstractPlugin& plugin, const std::string& options) {
    for(const std::string& option: Utility::String::splitWithoutEmptyParts(options, ',')) {
        auto keyValue = Utility::String::partition(option, '=');
        Utility::String::trimInPlace(keyValue[0]);
        Utility::String::trimInPlace(keyValue[2]);

        /* Provide a warning message in case the plugin doesn't define given
           option in its default config. The plugin is not *required* to have
           those tho (could be backward compatibility entries, for example), so
           not an error. */
        if(!plugin.configuration().valueCount(keyValue[0]))
            Warning{} << "Option" << keyValue[0] << "not recognized by" << plugin.plugin();

        /* If the option doesn't have an =, treat it as a boolean flag that's
           set to true. While there's no similar way to do an inverse, it's
           still nicer than causing a fatal error with those. */
        if(keyValue[1].empty())
            plugin.configuration().setValue(keyValue[0], true);
        else
            plugin.configuration().setValue(keyValue[0], keyValue[2]);
    }
}

}

int main(int argc, char** argv) {
    Utility::Arguments args;
    args.addArgument("input").setHelp("input", "input image")
        .addArgument("output").setHelp("output", "output image")
        .addOption("importer", "AnyImageImporter").setHelp("importer", "image importer plugin")
        .addOption("converter", "AnyImageConverter").setHelp("converter", "image converter plugin")
        .addOption("plugin-dir").setHelp("plugin-dir", "override base plugin dir", "DIR")
        .addOption('i', "importer-options").setHelp("importer-options", "configuration options to pass to the importer", "key=val,key2=val2,…")
        .addOption('c', "converter-options").setHelp("converter-options", "configuration options to pass to the converter", "key=val,key2=val2,…")
        .setGlobalHelp(R"(Converts images of different formats.

Specifying --importer raw:<format>` will treat the input as a raw
tightly-packed square of pixels in given pixel format. Specifying
--converter raw will save raw imported data instead of using a converter
plugin.

The -i / --importer-options and -c / --converter-options arguments accept a
comma-separated list of key/value pairs to set in the importer / converter
plugin configuration. If the = character is omitted, it's equivalent to saying
key=true.)")
        .parse(argc, argv);

    PluginManager::Manager<Trade::AbstractImporter> importerManager{
        args.value("plugin-dir").empty() ? std::string{} :
        Utility::Directory::join(args.value("plugin-dir"), Trade::AbstractImporter::pluginSearchPaths()[0])};

    /* Load raw data, if requested; assume it's a tightly-packed square of
       given format */
    /** @todo implement image slicing and then use `--slice "0 0 w h"` to
        specify non-rectangular size (and +x +y to specify padding?) */
    Containers::Optional<Trade::ImageData2D> image;
    if(Utility::String::beginsWith(args.value("importer"), "raw:")) {
        /** @todo Any chance to do this without using internal APIs? */
        const PixelFormat format = Utility::ConfigurationValue<PixelFormat>::fromString(args.value("importer").substr(4), {});
        const UnsignedInt pixelSize = Magnum::pixelSize(format);
        if(format == PixelFormat{}) {
            Error{} << "Invalid raw pixel format" << args.value("importer");
            return 4;
        }

        /** @todo simplify once read() reliably returns an Optional */
        if(!Utility::Directory::exists(args.value("input"))) {
            Error{} << "Cannot open file" << args.value("input");
            return 3;
        }
        Containers::Array<char> data = Utility::Directory::read(args.value("input"));
        auto side = Int(std::sqrt(data.size()/pixelSize));
        if(data.size() % pixelSize || side*side*pixelSize != data.size()) {
            Error{} << "File of size" << data.size() << "is not a tightly-packed square of" << format;
            return 5;
        }

        image = Trade::ImageData2D(format, Vector2i{side}, std::move(data));

    /* Otherwise load it using an importer plugin */
    } else {
        Containers::Pointer<Trade::AbstractImporter> importer = importerManager.loadAndInstantiate(args.value("importer"));
        if(!importer) {
            Debug{} << "Available importer plugins:" << Utility::String::join(importerManager.aliasList(), ", ");
            return 1;
        }

        /* Set options, if passed */
        setOptions(*importer, args.value("importer-options"));

        /* Open input file */
        if(!importer->openFile(args.value("input")) || !(image = importer->image2D(0))) {
            Error() << "Cannot open file" << args.value("input");
            return 3;
        }
    }

    {
        Debug d;
        if(args.value("converter") == "raw")
            d << "Writing raw image data of size";
        else
            d << "Converting image of size";
        d << image->size() << "and format";
        if(image->isCompressed()) d << image->compressedFormat();
        else d << image->format();
        d << "to" << args.value("output");
    }

    /* Save raw data, if requested */
    if(args.value("converter") == "raw") {
        Utility::Directory::write(args.value("output"), image->data());
        return 0;
    }

    /* Load converter plugin */
    PluginManager::Manager<Trade::AbstractImageConverter> converterManager{
        args.value("plugin-dir").empty() ? std::string{} :
        Utility::Directory::join(args.value("plugin-dir"), Trade::AbstractImageConverter::pluginSearchPaths()[0])};
    Containers::Pointer<Trade::AbstractImageConverter> converter = converterManager.loadAndInstantiate(args.value("converter"));
    if(!converter) {
        Debug{} << "Available converter plugins:" << Utility::String::join(converterManager.aliasList(), ", ");
        return 2;
    }

    /* Set options, if passed */
    setOptions(*converter, args.value("converter-options"));

    /* Save output file */
    if(!converter->exportToFile(*image, args.value("output"))) {
        Error() << "Cannot save file" << args.value("output");
        return 4;
    }
}
