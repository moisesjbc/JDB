#include <utilities/base_line.hpp>
#include <tinyxml2.h>
#include <gtest/gtest.h>


TEST(BaseLineTest, IsInitializedToZero) {
    jdb::BaseLine baseLine;

    EXPECT_EQ(0.0f, baseLine.x);
    EXPECT_EQ(0.0f, baseLine.y);
    EXPECT_EQ(0.0f, baseLine.width);
}


TEST(BaseLineTest, LoadsFromValidXML) {
    // Build a basic XML defining a base line and parse it.
    const char* xmlContent =
        "<base_line x=\"15.5\" y=\"31\" width=\"0.3\"></base_line>";
    tinyxml2::XMLDocument doc;
    doc.Parse(xmlContent);

    // Read the base line from previous XML.
    jdb::BaseLine baseLine;
    baseLine.loadFromXML(doc.FirstChildElement("base_line"));

    // Test results
    EXPECT_EQ(15.5f, baseLine.x);
    EXPECT_EQ(31, baseLine.y);
    EXPECT_EQ(0.3f, baseLine.width);
}
