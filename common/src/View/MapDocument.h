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

#ifndef __TrenchBroom__MapDocument__
#define __TrenchBroom__MapDocument__

#include "TrenchBroom.h"
#include "VecMath.h"
#include "Notifier.h"
#include "SharedPointer.h"
#include "StringUtils.h"
#include "Hit.h"
#include "Assets/EntityDefinitionManager.h"
#include "Assets/EntityModelManager.h"
#include "Assets/TextureManager.h"
#include "Controller/Command.h"
#include "IO/Path.h"
#include "Model/EntityDefinitionFileSpec.h"
#include "Model/IssueManager.h"
#include "Model/ModelFilter.h"
#include "Model/ModelTypes.h"
#include "Model/Picker.h"
#include "Model/PointFile.h"
#include "Model/Selection.h"
#include "Renderer/RenderConfig.h"
#include "View/CachingLogger.h"
#include "View/Grid.h"
#include "View/ViewTypes.h"

namespace TrenchBroom {
    class Logger;

    namespace Model {
        class BrushFace;
        class Layer;
        class Object;
        class Selection;
        class SelectionResult;
    }
    
    namespace View {
        class MapDocument : public CachingLogger {
        public:
            static const BBox3 DefaultWorldBounds;
        private:
            typedef std::tr1::weak_ptr<MapDocument> WkPtr;
            
            BBox3 m_worldBounds;
            IO::Path m_path;
            Model::GamePtr m_game;
            Model::Map* m_map;
            Assets::EntityDefinitionManager m_entityDefinitionManager;
            Assets::EntityModelManager m_entityModelManager;
            Assets::TextureManager m_textureManager;
            Assets::Texture* m_currentTexture;
            Model::Picker m_picker;
            Model::ModelFilter m_filter;
            Model::Selection m_selection;
            mutable BBox3 m_selectionBounds;
            mutable bool m_selectionBoundsValid;
            Model::IssueManager m_issueManager;
            Model::PointFile m_pointFile;
            Renderer::RenderConfig m_renderConfig;
            View::Grid m_grid;
            Model::Layer* m_currentLayer;
            
            bool m_textureLock;
            size_t m_modificationCount;
        public:
            Notifier0 documentWillBeClearedNotifier;
            Notifier0 documentWasClearedNotifier;
            Notifier0 documentWasNewedNotifier;
            Notifier0 documentWasLoadedNotifier;
            Notifier0 documentWasSavedNotifier;
            
            Notifier0 pointFileWasLoadedNotifier;
            Notifier0 pointFileWasUnloadedNotifier;

            Notifier0 modelFilterDidChangeNotifier;
            Notifier0 renderConfigDidChangeNotifier;
            
            Notifier1<const Model::ObjectList&> objectsWereAddedNotifier;
            Notifier1<const Model::ObjectList&> objectsWillBeRemovedNotifier;
            Notifier1<const Model::ObjectList&> objectsWereRemovedNotifier;
            Notifier1<const Model::ObjectList&> objectsWillChangeNotifier;
            Notifier1<const Model::ObjectList&> objectsDidChangeNotifier;
            
            Notifier5<Model::Entity*, const Model::PropertyKey&, const Model::PropertyValue&, const Model::PropertyKey&, const Model::PropertyValue&> entityPropertyDidChangeNotifier;
            
            Notifier1<const Model::BrushFaceList&> facesWillChangeNotifier;
            Notifier1<const Model::BrushFaceList&> facesDidChangeNotifier;
            
            Notifier1<const Model::LayerList&> layersWereAddedNotifier;
            Notifier1<const Model::LayerList&> layersWillBeRemovedNotifier;
            Notifier1<const Model::LayerList&> layersWereRemovedNotifier;
            
            Notifier0 modsDidChangeNotifier;
            Notifier0 entityDefinitionsDidChangeNotifier;
            Notifier0 textureCollectionsDidChangeNotifier;
            
            Notifier0 selectionWillChangeNotifier;
            Notifier1<const Model::SelectionResult&> selectionDidChangeNotifier;
        public:
            static MapDocumentSPtr newMapDocument();
            ~MapDocument();
            
            const BBox3& worldBounds() const;
            const IO::Path& path() const;
            String filename() const;

            Model::GamePtr game() const;
            Model::Map* map() const;
            const Model::ModelFilter& filter() const;
            Model::ModelFilter& filter();
            Assets::EntityDefinitionManager& entityDefinitionManager();
            Assets::EntityModelManager& entityModelManager();
            Assets::TextureManager& textureManager();
            Model::IssueManager& issueManager();
            Model::PointFile& pointFile();
            Renderer::RenderConfig& renderConfig();
            View::Grid& grid();
            
            Model::Layer* currentLayer() const;
            Model::Layer* layerForDuplicateOf(const Model::Object* object) const;
            void setCurrentLayer(Model::Layer* currentLayer);
            
            bool isGamePathPreference(const IO::Path& path) const;
            
            bool modified() const;
            void incModificationCount();
            void decModificationCount();
            void clearModificationCount();

            void newDocument(const BBox3& worldBounds, Model::GamePtr game, Model::MapFormat::Type mapFormat);
            void openDocument(const BBox3& worldBounds, Model::GamePtr game, const IO::Path& path);
            void saveDocument();
            void saveDocumentAs(const IO::Path& path);
            
            bool canLoadPointFile() const;
            void loadPointFile();
            bool isPointFileLoaded() const;
            void unloadPointFile();
            
            Model::EntityList parseEntities(const String& str) const;
            Model::BrushList parseBrushes(const String& str) const;
            Model::BrushFaceList parseFaces(const String& str) const;
            void writeObjectsToStream(const Model::ObjectList& objects, std::ostream& stream) const;
            void writeFacesToStream(const Model::BrushFaceList& faces, std::ostream& stream) const;
            
            Model::Entity* worldspawn();
            StringList mods() const;
            
            Model::EntityDefinitionFileSpec entityDefinitionFile() const;
            Model::EntityDefinitionFileSpec::List entityDefinitionFiles() const;
            
            void addExternalTextureCollections(const StringList& names);
            void removeExternalTextureCollections(const StringList& names);
            void moveExternalTextureCollectionUp(const String& name);
            void moveExternalTextureCollectionDown(const String& name);
            void updateExternalTextureCollectionProperty();

            void addEntities(const Model::EntityList& entities, const Model::ObjectLayerMap& layers);
            void addEntity(Model::Entity* entity, Model::Layer* layer);
            void removeEntities(const Model::EntityList& entities);
            void removeEntity(Model::Entity* entity);

            void addBrushes(const Model::EntityBrushesMap& brushes, const Model::ObjectLayerMap& layers);
            void addBrushes(Model::Entity* entity, const Model::BrushList& brushes, const Model::ObjectLayerMap& layers);
            void addBrush(Model::Entity* entity, Model::Brush* brush, Model::Layer* layer);
            void removeBrushes(const Model::BrushList& brushes);
            void removeBrush(Model::Brush* brush);

            void addLayers(const Model::LayerList& layers);
            void removeLayers(const Model::LayerList& layers);
            
            bool hasSelectedObjects() const;
            bool hasSelectedEntities() const;
            bool hasSelectedBrushes() const;
            bool hasSelectedFaces() const;
            bool hasSelection() const;
            const BBox3& selectionBounds() const;
            
            const Model::ObjectList& selectedObjects() const;
            const Model::EntityList& selectedEntities() const;
            const Model::BrushList& selectedBrushes() const;
            const Model::BrushFaceList& selectedFaces() const;
            
            Model::EntityList allSelectedEntities() const;
            Model::BrushList allSelectedBrushes() const;
            Model::BrushList selectedWorldBrushes() const;
            const Model::BrushFaceList& allSelectedFaces() const;

            Model::EntityList unselectedEntities() const;
            Model::BrushList unselectedBrushes() const;
            
            Model::SelectionResult selectObjects(const Model::ObjectList& objects);
            Model::SelectionResult deselectObjects(const Model::ObjectList& objects);
            Model::SelectionResult selectAllObjects();
            Model::SelectionResult selectAllFaces();
            Model::SelectionResult selectFaces(const Model::BrushFaceList& faces, bool keepBrushSelection);
            Model::SelectionResult deselectFaces(const Model::BrushFaceList& faces);
            Model::SelectionResult deselectAll();
            
            Assets::Texture* currentTexture() const;
            String currentTextureName() const;
            void setCurrentTexture(Assets::Texture* texture);
            
            bool textureLock() const;
            void setTextureLock(const bool textureLock);
            
            void commitPendingRenderStateChanges();

            Hits pick(const Ray3& ray);
            
            void saveBackup(const IO::Path& path);
        private:
            MapDocument();
            
            void bindObservers();
            void unbindObservers();
            
            void registerIssueGenerators();
            void reloadIssues();
            
            void selectionDidChange(const Model::SelectionResult& selection);
            
            class ObjectWasAdded : public Model::ObjectVisitor {
            private:
                MapDocument& m_document;
            public:
                ObjectWasAdded(MapDocument& document);
                void doVisit(Model::Entity* entity);
                void doVisit(Model::Brush* brush);
            };
            
            class ObjectWillBeRemoved : public Model::ObjectVisitor {
            private:
                MapDocument& m_document;
            public:
                ObjectWillBeRemoved(MapDocument& document);
                void doVisit(Model::Entity* entity);
                void doVisit(Model::Brush* brush);
            };

            class ObjectWasRemoved : public Model::ObjectVisitor {
            private:
                MapDocument& m_document;
            public:
                ObjectWasRemoved(MapDocument& document);
                void doVisit(Model::Entity* entity);
                void doVisit(Model::Brush* brush);
            };
            
            class ObjectWillChange : public Model::ObjectVisitor {
            private:
                MapDocument& m_document;
            public:
                ObjectWillChange(MapDocument& document);
                void doVisit(Model::Entity* entity);
                void doVisit(Model::Brush* brush);
            };
            
            class ObjectDidChange : public Model::ObjectVisitor {
            private:
                MapDocument& m_document;
            public:
                ObjectDidChange(MapDocument& document);
                void doVisit(Model::Entity* entity);
                void doVisit(Model::Brush* brush);
            };
            
            void objectsWereAdded(const Model::ObjectList& objects);
            void objectsWillBeRemoved(const Model::ObjectList& objects);
            void objectsWereRemoved(const Model::ObjectList& objects);
            void objectsWillChange(const Model::ObjectList& objects);
            void objectsDidChange(const Model::ObjectList& objects);
            void updateLinkSourcesInIssueManager(Model::Entity* entity);
            
            void entityPropertyDidChange(Model::Entity* entity, const Model::PropertyKey& oldKey, const Model::PropertyValue& oldValue, const Model::PropertyKey& newKey, const Model::PropertyValue& newValue);
            
            void facesDidChange(const Model::BrushFaceList& faces);
            
            void modsDidChange();
            void entityDefinitionsDidChange();
            void textureCollectionsDidChange();
            void preferenceDidChange(const IO::Path& path);
            
            void clearMap();
            
            void updateGameSearchPaths();
            void loadAndUpdateEntityDefinitions();
            void loadEntityDefinitions();
            void unloadEntityDefinitions();
            void clearEntityModels();
            void updateEntityDefinitions(const Model::EntityList& entities);
            void updateEntityDefinition(Model::Entity* entity);
            void updateEntityModels(const Model::EntityList& entities);
            void updateEntityModel(Model::Entity* entity);
            void setEntityModel(Model::Entity* entity);

            void loadAndUpdateTextures();
            void loadTextures();
            void loadBuiltinTextures();
            void loadExternalTextures();
            
            void updateTextures();
            void setTextures(const Model::BrushFaceList& faces);
            void setTexture(Model::BrushFace* face);
            void unsetTextures(const Model::BrushFaceList& faces);
            void unsetTexture(Model::BrushFace* face);
            
            void doAddExternalTextureCollections(const StringList& names);
            IO::Path::List externalSearchPaths() const;
            
            void doSaveDocument(const IO::Path& path);
            void setDocumentPath(const IO::Path& path);
        };
    }
}

#endif /* defined(__TrenchBroom__MapDocument__) */
