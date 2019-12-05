// Copyright 2019 Stellar Development Foundation and contributors. Licensed
// under the Apache License, Version 2.0. See the COPYING file at the root
// of this distribution or at http://www.apache.org/licenses/LICENSE-2.0

#include "bucket/Bucket.h"
#include "ledger/test/LedgerTestUtils.h"
#include "lib/catch.hpp"
#include "lib/util/format.h"
#include "test/test.h"
#include "util/Logging.h"
#include "util/XDRStream.h"

#include <chrono>

using namespace stellar;

TEST_CASE("XDROutputFileStream fail modes", "[xdrstream]")
{
    XDROutputFileStream out(/*doFsync=*/true);
    auto filename = "someFile";

    SECTION("open throws")
    {
        REQUIRE_NOTHROW(out.open(filename));
        // File is already open
        REQUIRE_THROWS_AS(out.open(filename), std::runtime_error);
        std::remove(filename);
    }
    SECTION("write throws")
    {
        auto hasher = SHA256::create();
        size_t bytes = 0;
        auto ledgerEntries = LedgerTestUtils::generateValidLedgerEntries(1);
        auto bucketEntries =
            Bucket::convertToBucketEntry(false, {}, ledgerEntries, {});

        REQUIRE_THROWS_AS(out.writeOne(bucketEntries[0], hasher.get(), &bytes),
                          std::runtime_error);
    }
    SECTION("close throws")
    {
        REQUIRE_THROWS_AS(out.close(), std::runtime_error);
    }
}

TEST_CASE("XDROutputFileStream fsync bench", "[!hide][xdrstream][bench]")
{
    Config const& cfg = getTestConfig(0);

    auto hasher = SHA256::create();
    auto ledgerEntries = LedgerTestUtils::generateValidLedgerEntries(10000000);
    auto bucketEntries =
        Bucket::convertToBucketEntry(false, {}, ledgerEntries, {});

    fs::mkpath(cfg.BUCKET_DIR_PATH);

    for (int i = 0; i < 10; ++i)
    {
        XDROutputFileStream outFsync(/*doFsync=*/true);
        XDROutputFileStream outNoFsync(/*doFsync=*/false);

        outFsync.open(
            fmt::format("{}/outFsync-{}.xdr", cfg.BUCKET_DIR_PATH, i));
        outNoFsync.open(
            fmt::format("{}/outNoFsync-{}.xdr", cfg.BUCKET_DIR_PATH, i));

        size_t bytes = 0;
        auto start = std::chrono::system_clock::now();
        for (auto const& e : bucketEntries)
        {
            outFsync.writeOne(e, hasher.get(), &bytes);
        }
        outFsync.close();
        auto stop = std::chrono::system_clock::now();
        auto elapsed =
            std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        CLOG(INFO, "Fs") << "wrote " << bytes << " bytes to fsync file in "
                         << elapsed.count() << "ms";

        bytes = 0;
        start = std::chrono::system_clock::now();
        for (auto const& e : bucketEntries)
        {
            outNoFsync.writeOne(e, hasher.get(), &bytes);
        }
        outNoFsync.close();
        stop = std::chrono::system_clock::now();
        elapsed =
            std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        CLOG(INFO, "Fs") << "wrote " << bytes << " bytes to no-fsync file in "
                         << elapsed.count() << "ms";
    }
}
