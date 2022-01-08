#include <iostream>
#include <vector>
#include <memory>

#include "mainWindow.hpp"
#include "cloneRepoDialog.hpp"

namespace gitgui {
	
	constexpr char tabViewName[] = {"View"};
	constexpr char tabEditName[] = {"Edit"};
	constexpr char errorPathRepositoryMsg[] = {"Select path dosen't have any repository!"};

	// Menu items ID/name for start menu Window
	static std::vector<MenuItem> startMenuItems {
		MenuItem{MOVE_TO_REPO, QString{"Move to the repository directory"}},
		MenuItem{CLONE_NEW_REPO, QString{"Clone new repository"}}
	};

	// MainWindow methods implementations
	MainWindow::MainWindow(QWidget *wgtptr)
		: QMainWindow(wgtptr)
	{
		createStartMenuWindow();
		connect(mptrStartWindow, &StartWindow::lineClicked, this, &MainWindow::handleStartMenuSelectItem);
	}

	// INIT methods
	void MainWindow::createStartMenuWindow() {
		mptrStartWindow = new StartWindow(startMenuItems);
		this->setCentralWidget(mptrStartWindow);
		fixedCentralWgt(mptrStartWindow->size());
	}

	void MainWindow::handleStartMenuSelectItem(int menuType) {
		switch(menuType) {
			case StartMenuType::MOVE_TO_REPO:
				createRepository(QFileDialog::getExistingDirectory(0, "Select Directory", "."));
				break;
			case StartMenuType::CLONE_NEW_REPO:
				cloneNewRepository();
				break;
		}
	}
	
	void MainWindow::createMainRepoWindow() {
		mptrTab = new QTabWidget;
		mptrViewWindow = new ViewGitWindow();
		mptrTab->addTab(mptrViewWindow, QString(tabViewName));
		mptrTab->addTab(new QLabel(), QString(tabEditName));
		this->setCentralWidget(mptrTab);
		relaxCentralWgt();
	}
	
	void MainWindow::createRepository(const QString& path) {
		if (path.isEmpty())
			return;
		try {
			mGitRepo = std::shared_ptr<GitRepository>(new GitRepository(path));
			createMainRepoWindow();
			gitViewRepoConnections();
			mGitRepo->startInitVeiwWindow();
		} catch(std::invalid_argument &except) {
			QMessageBox::warning(0, "Error!", errorPathRepositoryMsg, QMessageBox::Ok);
			if (mGitRepo)
				mGitRepo.reset();
		}
	}
	
	void MainWindow::gitViewRepoConnections() {
		connect(mGitRepo.get(), &GitRepository::setModelCommit, mptrViewWindow, &ViewGitWindow::setModelCommit);
		connect(mGitRepo.get(), &GitRepository::setModelBranch, mptrViewWindow, &ViewGitWindow::setModelBranch);
		connect(mGitRepo.get(), &GitRepository::showCommitChanges, mptrViewWindow, &ViewGitWindow::showCommitChanges);
		connect(mptrViewWindow, &ViewGitWindow::commitForShowChanges, mGitRepo.get(), &GitRepository::commitForShowChanges);
		connect(mptrViewWindow, &ViewGitWindow::makeCheckout, mGitRepo.get(), &GitRepository::makeCheckout);
		connect(mGitRepo.get(), &GitRepository::showActiveCommit, mptrViewWindow, &ViewGitWindow::showActiveCommit);
		connect(mGitRepo.get(), &GitRepository::showActiveBranch, mptrViewWindow, &ViewGitWindow::showActiveBranch);
	}
	
	void MainWindow::cloneNewRepository() {
		std::unique_ptr<CloneRepoDialog> cloneRepoDlg(new CloneRepoDialog);
		if (cloneRepoDlg->exec() == QDialog::Accepted) {
			QTextStream out(stdout);
			out << "URL: " << cloneRepoDlg->getURL() << endl;
			out << "Directory: " << cloneRepoDlg->getDir() << endl;
		}
	}
	
	void MainWindow::fixedCentralWgt(const QSize& size) {
		if (size.isValid())
			this->setFixedSize(mptrStartWindow->size());
	}
	
	void MainWindow::relaxCentralWgt() {
		this->setMinimumSize(600, 400);
		this->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
	}
	
	
	
}
