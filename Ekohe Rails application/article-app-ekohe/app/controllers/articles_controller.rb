class ArticlesController < ApplicationController
  before_action :find_article, only: [:destroy, :update, :edit, :show]
  before_action :search_article
  before_action :set_all_users_and_articles
  def index
    # order all articles to be in id descending order
    @user = current_user
  end

  def show
    # find the user associated with id
    @user = User.find(@article.user_id)

    # increase number of views with each click on show then save results
    # only if it is public or if you are the owner if private
    if @article.isPublic || (!@article.isPublic && @article.user_id == current_user.id)
      @article.view_count += 1
      @article.save
    end
  end

  def new
    @article = Article.new
  end

  def create
    @article = Article.new(article_params)
    @article.user_id = current_user.id


    if @article.save
      redirect_to root_path
    else
      puts @article.errors.full_messages
      render :new
    end
  end

  def edit
  end

  def update
    if @article.update(article_params)
      redirect_to article_path(@article)
    else
      render :index
    end
  end

  def destroy
    @article.destroy
    redirect_to root_path, notice: 'article was successfully destroyed.'
  end


  private

  def article_params
    params.require(:article).permit(:title, :content, :publishDate, :isPublic)
  end

  def find_article
    @article = Article.find(params[:id])
  end

  def search_article
    if params[:query].present?
      # .joins(:user)
      @article_search = PgSearch.multisearch(params[:query])
    end
  end

  def set_all_users_and_articles
    @all_users = User.all
    @articles = Article.all.order(id: :desc)
  end

end
