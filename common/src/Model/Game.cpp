/*
 Copyright (C) 2010-2014 Kristian Duske
 
 This file is part of TrenchBroom.
 
 TrenchBroom is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 TrenchBroom is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with TrenchBroom. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Game.h"

#include "Model/BrushContentTypeBuilder.h"
#include "Model/GameFactory.h"

namespace TrenchBroom {
    namespace Model {
        Game::Game() :
        m_brushContentTypeBuilder(NULL) {}
        
        Game::~Game() {
            delete m_brushContentTypeBuilder;
            m_brushContentTypeBuilder = NULL;
        }

        const String& Game::gameName() const {
            return doGameName();
        }

        bool Game::isGamePathPreference(const IO::Path& prefPath) const {
            const GameFactory& gameFactory = GameFactory::instance();
            return gameFactory.isGamePathPreference(gameName(), prefPath);
        }

        IO::Path Game::gamePath() const {
            return doGamePath();
        }

        void Game::setGamePath(const IO::Path& gamePath) {
            doSetGamePath(gamePath);
        }

        void Game::setAdditionalSearchPaths(const IO::Path::List& searchPaths) {
            doSetAdditionalSearchPaths(searchPaths);
        }

        World* Game::newMap(const MapFormat::Type format) const {
            return doNewMap(format);
        }
        
        World* Game::loadMap(const BBox3& worldBounds, const IO::Path& path, Logger* logger) const {
            return doLoadMap(worldBounds, path, logger);
        }

        void Game::writeMap(World* world, const IO::Path& path) const {
            assert(world != NULL);
            doWriteMap(world, path);
        }

        /*
        Model::EntityList Game::parseEntities(const BBox3& worldBounds, MapFormat::Type format, const String& str) const {
            return doParseEntities(worldBounds, format, str);
        }
        
        Model::BrushList Game::parseBrushes(const BBox3& worldBounds, MapFormat::Type format, const String& str) const {
            return doParseBrushes(worldBounds, format, str);
        }
        
        Model::BrushFaceList Game::parseFaces(const BBox3& worldBounds, MapFormat::Type format, const String& str) const {
            return doParseFaces(worldBounds, format, str);
        }
         */
        
        void Game::writeSelectedNodesToStream(World* world, std::ostream& stream) const {
            doWriteSelectedNodesToStream(world, stream);
        }
    
        void Game::writeBrushFacesToStream(const Model::BrushFaceList& faces, const MapFormat::Type format, std::ostream& stream) const {
            doWriteBrushFacesToStream(faces, format, stream);
        }
    
        bool Game::isTextureCollection(const IO::Path& path) const {
            return doIsTextureCollection(path);
        }

        IO::Path::List Game::findBuiltinTextureCollections() const {
            return doFindBuiltinTextureCollections();
        }
        
        StringList Game::extractExternalTextureCollections(const World* world) const {
            assert(world != NULL);
            return doExtractExternalTextureCollections(world);
        }
        
        void Game::updateExternalTextureCollections(World* world, const StringList& collections) const {
            assert(world != NULL);
            doUpdateExternalTextureCollections(world, collections);
        }

        bool Game::isEntityDefinitionFile(const IO::Path& path) const {
            return doIsEntityDefinitionFile(path);
        }

        Assets::EntityDefinitionFileSpec::List Game::allEntityDefinitionFiles() const {
            return doAllEntityDefinitionFiles();
        }

        Assets::EntityDefinitionFileSpec Game::extractEntityDefinitionFile(const World* world) const {
            assert(world != NULL);
            return doExtractEntityDefinitionFile(world);
        }
        
        IO::Path Game::findEntityDefinitionFile(const Assets::EntityDefinitionFileSpec& spec, const IO::Path::List& searchPaths) const {
            return doFindEntityDefinitionFile(spec, searchPaths);
        }
        
        const BrushContentTypeBuilder* Game::brushContentTypeBuilder() const {
            if (m_brushContentTypeBuilder == NULL)
                m_brushContentTypeBuilder = new BrushContentTypeBuilder(brushContentTypes());
            return m_brushContentTypeBuilder;
        }

        const BrushContentType::List& Game::brushContentTypes() const {
            return doBrushContentTypes();
        }

        StringList Game::availableMods() const {
            return doAvailableMods();
        }

        StringList Game::extractEnabledMods(const World* world) const {
            assert(world != NULL);
            return doExtractEnabledMods(world);
        }
        
        const GameConfig::FlagsConfig& Game::surfaceFlags() const {
            return doSurfaceFlags();
        }
        
        const GameConfig::FlagsConfig& Game::contentFlags() const {
            return doContentFlags();
        }
    }
}
